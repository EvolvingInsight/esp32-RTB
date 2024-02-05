#include "FlashMem.h"

/**
 * Initialize the storage filesystem
*/
bool FlashMem::init(){
    
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
        return false;
        
    return true;
}

/**
 * Verify that the given file exist.
 *
 * @param fileName File name.
 * @return True, if the file exists, false otherwise.
 */
bool FlashMem::fileExist(const std::string fileName){

    std::string path = fileName; // not a necessity, but it shows that the file is located at the root of the filesystem
    path.insert(0,"/");

    if(LittleFS.exists(path.c_str()))
        return true;

    return false;
}

/**
 * Creates a file with the name provided.
 *
 * @param fileName File name.
 * @return True, if the file has been create, false otherwise.
 */
bool FlashMem::createFile(const std::string fileName){

    std::string path = fileName;
    path.insert(0,"/");
    
    fs::File f = LittleFS.open(path.c_str(), "w");

    if(!f)
        return false;

    f.close();

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
 * @return True, if the operation was successful, false if the file couldn't be opened.
 */
bool FlashMem::readFrom(const std::string fileName, std::vector<std::string>& vect){

    std::string path = fileName;
    path.insert(0,"/");

    fs::File f = LittleFS.open(path.c_str(), "r");

    if(!f)
        return false;
    
    std::string stemp;

    // Get all the data
    while(f.available()){
        stemp = f.readStringUntil('\n').c_str();

        if(stemp.length() > 10)
            vect.push_back(stemp.c_str());
    }

    f.close();

    return true;
}

/**
 * Append a line to a file.
 *
 * @param fileName File name.
 * @param line Line of text to be added. It puts an /n character after the line to be written.
 * @return True, if the operation was successful, false if the file name doesn't exist.
 */
bool FlashMem::addLine(const std::string fileName, const std::string line){

    std::string path = fileName;
    path.insert(0,"/");

    fs::File f = LittleFS.open(path.c_str(), "a");

    if(!f)
        return false;
    
    f.println(line.c_str());

    f.close();

    return true;
}

/**
 * Erase every data of a file.
 *
 * @param fileName File's name that content will be erased.
 * @return True, if the operation was successful, false if the file name doesn't exist.
 */
bool FlashMem::clearFile(const std::string fileName){

    std::string path = fileName;
    path.insert(0,"/");

    fs::File f = LittleFS.open(path.c_str(), "w");

    if(!f)
        return false;

    return true;
}