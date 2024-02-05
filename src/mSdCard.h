
#ifndef MSdCard_H
#define MSdCard_H

#include "Storage.h"

#include <SD.h>
#include <SdFat.h>      // Library for SD Card data storing

/**
 * Child class of Storage
*/
class mSdCard : public Storage{
private:
    // SD variables
    SdFat sd;
    SdFile registre;
    char line[25];

public:
    mSdCard() = default;                           // Constructor & destructor
    mSdCard(const mSdCard &u) = delete;   // Deletion of copy constructor, security for assuring there's only one instance

    ~mSdCard() = default;

    bool init() override;
    
    bool fileExist(const std::string registreName) override;
    bool createFile(const std::string fileName) override;
    bool readFrom(const std::string fileName, std::vector<std::string>& vect) override;
    bool addLine(const std::string fileName, const std::string line) override;
    bool clearFile(const std::string fileName) override;
};


#endif