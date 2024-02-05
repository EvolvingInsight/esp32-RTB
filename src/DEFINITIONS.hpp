#pragma once
#include "RTClib.h"				// Library for RTC

/*******************************************
 * Section intended to be modified
*******************************************/

/** User creation, if you wish to add another user, add a line in UsersPrep
 *  Make sure that no password are the same, number 1 to 3 only, any size is allowed.
 *  Username is one character only. */

/** User parameters */
struct UsersConfig { const char username; const std::string password; const int tokens;};
const UsersConfig UsersPrep[] = {
	{'a', "123123", (int)2 },
};

/**
 * On first time use, set BOTH to "true" if it's your case, remember to change both to false afterwards
 */

/** Set the RTC value from computer time */
#define SET_RTC_TIME false

/** Format littlefs so you can use internal memory */
#define FORMAT_LITTLEFS_IF_FAILED false

/** Is the amount of time before another token is needed to open the lock */
const TimeSpan ActivatedTime(1,0,0,0);

/** 
 * Long term memory configuration (warning : if these two are set to false simultaneously an error will be thrown)
 */

/**  Choose whether to log (everytime the safe box (RTB) gets open) in the internal memory instead of the sd card
 *   Also if set to true, don't forget to upload the filesystem (littlefs) */
#define USE_INTERNAL_MEMORY true

/** Choose if you want to instantiate the sd card class or not,
 *  false also means that no log for errors will be created	*/
#define USE_SD_CARD true

/** If you want debug information to be print in console */
#define DEBUG_ENABLED false

const std::string Registry = "registre.txt";			// Registry file name, where usage will be saved
const std::string ErrorLog = "log.txt";					// Log file name (contains any occuring error)


// Pins configuration
#define SD_CHIP_SELECT_PIN 5
#define Button1 12	// GPIO12 of ESP32
#define Button2 13	// GPIO13 of ESP32
#define Button3 14	// GPIO14 of ESP32
#define EnterPin 15	// GPIO15 of ESP32
#define LockPin 4	// GPIO04 of ESP32

/*******************************************
 * Section not intended to be modified
*******************************************/

#define SPI_SPEED_RATE SD_SCK_MHZ(21)   // Set SPI rate to 21MHz, otherwise SPI_FULL_SPEED causes to much distortion to the SPI signal

/**
 * Use to represent RTB State : helps managing permission or problem.
 */
enum State{
	Ready,			// Beginning state, ready to be activate.
	Activated,		// Temporary state, meaning it has been activated and could be open as many time as needed for e.g. 24 hours.
	Problem,		// Technical problem with either : RTC, SD card or semantic logic.
};