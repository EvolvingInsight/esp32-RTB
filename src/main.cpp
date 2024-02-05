/**************************************************************************************
Program :   main.cpp
Author :    Loïc Bouillon
Date :      03/06/2022
Purpose :   Control an ESP32 and two other "complex" components.
**************************************************************************************/
// Preprocessor directives
#include <Arduino.h>
#include <Wire.h>				// Library for I2C communication
#include <SPI.h>				// Library for SPI communication

#include "StorageManagement.h"	// Includes all necessary file manager
#include "User.hpp"				// Class that holds user data

#if USE_INTERNAL_MEMORY && USE_SD_CARD
	Storage * usageStorage = new FlashMem();
	Storage * logStorage = new mSdCard();
#elif USE_INTERNAL_MEMORY && !USE_SD_CARD
	Storage * usageStorage = new FlashMem();
	Storage * logStorage = new NullStorage();
#elif !USE_INTERNAL_MEMORY && USE_SD_CARD
	Storage * usageStorage = new SdCard();
	Storage * logStorage = usageStorage;
#else
	#error At least one storage must be set to true (see: DEFINITIONS.hpp)
#endif


#if DEBUG_ENABLED
	#define debug(x) Serial.print(x)
	#define debugln(x) Serial.println(x)
#else
	#define debug(x)
	#define debugln(x)
#endif

/**
 * Contains all the data e.g. a2020-06-25T15:29:37
 * and also, without the first letter (representing the user whom has accessed),
 * it respects the format: iso8601dateTime
*/
std::vector<std::string> lines;

// Error message to be logged into the log file
std::string logErrorMessage = "";

// Instance of DS3231
RTC_DS3231 RTC;

// Container for all users
std::vector<User> users;

// Current state of the RTB
int CurrentState;


void setRTCtime();
void updateUserTokens();
void updateState();
std::string getUserInput();
int returnUserIndex(std::string input);
void incrementUsedTokens(int index);
void openLock(int delayMillisec);

void setup() {
	/*******************************************
		SETUP of esp32 and communication
	*******************************************/
 	Serial.begin(9600);						// Communication rate
	
	Wire.begin();							// Start the I2C
	
	delay(3000);							// Wait for console opening

	// Setup of pinMode
	pinMode(Button1, INPUT_PULLDOWN);
	pinMode(Button2, INPUT_PULLDOWN);
	pinMode(Button3, INPUT_PULLDOWN);
	pinMode(EnterPin, INPUT_PULLDOWN);
	pinMode(LockPin, OUTPUT);

	/*******************************************
			SETUP of physical components
	*******************************************/
	
	// Init RTC
	if (!RTC.begin()){
		CurrentState = State::Problem;
		logErrorMessage += "\nCouldn't find RTC.";
	}
	else{
		#if SET_RTC_TIME
			setRTCtime();			// Set Time for RTC, do it once
		#endif
	}
	
	debug("\nRTC date : ");
	debugln(RTC.now().timestamp(DateTime::TIMESTAMP_FULL).c_str());

	// Init of storage
	if(!usageStorage->init()){
		CurrentState = State::Problem;
		logErrorMessage += "\nCouldn't initialize usage storage";
	}

	if(!logStorage->init()){
		CurrentState = State::Problem;
		logErrorMessage += "\nCouldn't initialize log storage";
	}

	// Check if the registry file exist, otherwise creates it
	if(!usageStorage->fileExist(Registry))
		if(!usageStorage->createFile(Registry)){
			CurrentState = State::Problem;
			logErrorMessage += "\nCouldn't create registry file.";
		}
	
	// Same here for the log file
	if(!logStorage->fileExist(ErrorLog)){
		if(!logStorage->createFile(ErrorLog)){
			CurrentState = State::Problem;
			logErrorMessage += "\nCouldn't create log file.";
		}
	}

	/*******************************************
					SETUP of users
	*******************************************/
	// User assignment
	for (struct UsersConfig x : UsersPrep) {
		User tempUser;
		tempUser.init(x.username,x.password,x.tokens);
		users.push_back(tempUser);
	}

	/*******************************************
		UPDATE data from registry file
	*******************************************/
	if(!usageStorage->readFrom(Registry,lines)){
		CurrentState = State::Problem;
		logErrorMessage += "\nCouldn't read data from registry file.";
	}
	// Debug message, to see what is stored in "lines" variable
	for (string line : lines)
		debugln(line.c_str());

	updateUserTokens();			// Update of used token
}

void loop() {
	// Check if there's any hardware or setup problem, if so, it opens the lock
	if(CurrentState == State::Problem){

		// Log the error
		logStorage->addLine(ErrorLog, logErrorMessage.c_str());

		debug("\nCurrent State : problem, error message :\n\n");
		debugln(logErrorMessage.c_str());

		// Open lock every 30 seconds, for 2 seconds.
		while(true){
			openLock(2000);
			delay(30000);
		}
	}
	
	int uIndex = -1; // Index representing the current user
	
	// Get user input
	std::string input = getUserInput();

	// Find user based on input
  	uIndex = returnUserIndex(input);
	
	if(uIndex != -1){   // True if user exist
		
		updateState();	// Update of current State
			
		// True if : user is allowed or the RTB is active and there is no technical problem
		if((users[uIndex].isAllowed() || CurrentState == State::Activated) && CurrentState != State::Problem){

			if(CurrentState == State::Ready){
				incrementUsedTokens(uIndex);
				openLock(3000);
			}
			else{// CurrentState activated
				openLock(3000);
			}
		}
	}
}

/**
 * Time and date of the RTC are set from time on computer at compiletime.
 *
 * @return Void.
 */
void setRTCtime(){
	RTC.adjust(DateTime(__DATE__, __TIME__));

	debug("\nTime and date set to : ");
	debugln(DateTime(__DATE__, __TIME__).timestamp(DateTime::TIMESTAMP_FULL).c_str());
}


/**
 * Update all users used tokens from "lines" vector.
 * 
 * @return Void.
 */
void updateUserTokens(){
	for (User& x : users)
		for (string line : lines)
			if(line.at(0) == x.getLetter())
				x.addUsedTokens();
}


/**
 * Update the state of the RTB, based on global variable "lines".
 *
 * @return Void.
 */
void updateState(){

	if(!RTC.now().isValid()){
		CurrentState = State::Problem;
		logErrorMessage += "\nRTC date is not valid.";
		return;
	}

	// If there is no data
	if(lines.empty()){
		CurrentState = State::Ready;
		return;
	}

	// Get last line from the "lines" and
	// convert it to DateTime object
	DateTime lastDate = DateTime(lines.back().substr(1).c_str());//	Respect the format: iso8601dateTime
	if(!lastDate.isValid()){
		CurrentState = State::Problem;
		logErrorMessage += "\nDate in global variable \"lines\" isn't valid, see the registry file.";
		return;
	}

	// Verify if it has been activated recently
	if((lastDate + ActivatedTime) >= RTC.now()){
		CurrentState = State::Activated;
		return;
	}
	
	// If time elapsed since last activation is greater than one month or one year, renew tokens
	if(lastDate.month() < RTC.now().month() || lastDate.year() < RTC.now().year()){
		if(!usageStorage->clearFile(Registry)){
			CurrentState = State::Problem;
			logErrorMessage += "\nStorage can't be cleared.";
		}
			
		lines.clear();
		
		for (User& x : users)
			x.resetUsedTokens();
	}

	CurrentState = State::Ready;
}


/**
 * Loop that waits for the user input. Could be replace by an input coming from a web server.
 *
 * @return Whole user input as a string.
 */
std::string getUserInput(){
	std::string input = "";

	int Button1State, Button2State, Button3State = 0;
	while(digitalRead(EnterPin) != HIGH){
		Button1State = digitalRead(Button1);
		Button2State = digitalRead(Button2);
		Button3State = digitalRead(Button3);

    
		// Debouncing-like function
		if(Button1State==HIGH){
			while (Button1State == HIGH)
			{
				delay(50);
				Button1State = digitalRead(Button1);
			}
			input += '1';
		}
		else if(Button2State==HIGH){
			while (Button2State == HIGH)
			{
				delay(50);
				Button2State = digitalRead(Button2);
			}
			input += '2';
		}
		else if(Button3State==HIGH){
			while (Button3State == HIGH)
			{
				delay(50);
				Button3State = digitalRead(Button3);
			}
			input += '3';
		}
		
		delay(50);
	}

	return input;
}

/**
 * Verify that the input correspond to a password. If it doesn't, the return will be -1.
 *
 * @param input Must match a user's password.
 * @return returns an int, as an index of the corresponding user.
 */
int returnUserIndex(std::string input){
	int index = 0;

	for (User x : users){
		if(x.getPwd() == input.c_str())
			return index;
		index++;
	}
	return -1;
}

/**Registry
 * Increment the number of used tokens and write to the storage.
 * Also, it appends a line to lines global variable.
 *
 * @param index Index representing the user in Users global variable.
 * @return Void.
 */
void incrementUsedTokens(int index){
	std::string nLine;     // nLine will be written in the registry file

	nLine.push_back(users[index].getLetter());

	nLine += RTC.now().timestamp(DateTime::TIMESTAMP_FULL).c_str();

	if(!usageStorage->addLine(Registry,nLine.c_str())){
		CurrentState = State::Problem;
		logErrorMessage += "\nCannot append a line in the registry file.";
	}
	
	lines.push_back(nLine);
	
	users[index].addUsedTokens();

	debug("\nUser has used one token.");
}

/**
 * Opens the lock pins for a given time.
 *
 * @param delayMillisec Duration of the delay in milliseconds.
 * @return Void.
 */
void openLock(int delayMillisec){
	digitalWrite(LockPin,HIGH);
	delay(delayMillisec);
	digitalWrite(LockPin,LOW);
}