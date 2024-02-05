#include "mSdCard.h"

/**
 * Initialize the storage filesystem or print a detailed error message and halt
*/
bool mSdCard::init(){
    
  	if (!sd.begin(SD_CHIP_SELECT_PIN, SPI_SPEED_RATE)){
        sd.initErrorPrint();
        return false;
    }
        
    return true;
}

/**
 * Verify that the given file exist.
 *
 * @param fileName File name.
 * @return True, if the file exists, false otherwise.
 */
bool mSdCard::fileExist(const std::string fileName){

    // Open file for reading
  	if (!registre.open(fileName.c_str(), O_RDONLY))
        return false;

	registre.close();

    return true;
}

/**
 * Creates a file with the name supplied.
 *
 * @param fileName File name.
 * @return True, if the file has been create, false otherwise.
 */
bool mSdCard::createFile(const std::string fileName){

    if(!registre.open(fileName.c_str(), O_RDWR | O_CREAT))
        return false;

    registre.close();

    return true;
}

/**
 * Reads and add all lines in a vector of string, until it reaches a int value n of zero.
 * 
 * If you manually put data into a file, make sure
 * that you press Enter at the end of each string.
 * 
 * @param fileName File name where data are written.
 * @param vect Vector that will be filled with every line found.
 * @return True, if the operation was successful, false if the file name doesn't exist.
 */
bool mSdCard::readFrom(const std::string fileName, std::vector<std::string>& vect){

    if(!registre.open(fileName.c_str(), O_RDONLY))
        return false;
    
    int n;
    while ((n = registre.fgets(line, sizeof(line))) > 0) {	// Add SD card line to lines
        if (line[n - 1] == '\n') {// && != ''
            line[n - 1] = 0;// Set \n character to nothing
            vect.push_back(line);
        }else{
            vect.push_back(line);       // TODO: Check if it works // If there's no /n char at the end, but OEF is encountered
        }
    }

    registre.close();

    return true;
}

/**
 * Append a line to a file.
 *
 * @param fileName File name.
 * @param line Line of text to be added. It puts an /n character after the line to be written.
 * @return True, if the operation was successful, false if the file name doesn't exist.
 */
bool mSdCard::addLine(const std::string fileName, const std::string line){

    if(!registre.open(fileName.c_str(), O_APPEND | O_WRITE))
        return false;

    registre.println(line.c_str());

    registre.close();

    return true;
}

/**
 * Erase every data of a file.
 *
 * @param fileName File's name that content will be erased.
 * @return True, if the operation was successful, false if the file name doesn't exist.
 */
bool mSdCard::clearFile(const std::string fileName){

    if(!registre.open(fileName.c_str(), O_RDWR))
        return false;

    registre.truncate(0);

    registre.close();

    return true;
}