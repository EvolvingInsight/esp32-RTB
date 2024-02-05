
#ifndef FLASHMEM_H
#define FLASHMEM_H

#include "Storage.h"

#include <LittleFS.h>	    // Library for internal storage TODO : remove this (?????)

/**
 * Child class of Storage
*/
class FlashMem : public Storage{

public:
    FlashMem() = default;                           // Constructor & destructor
    FlashMem(const FlashMem &u) = delete;   // Deletion of copy constructor, security for assuring there's only one instance

    ~FlashMem() = default;

    bool init() override;
    
    bool fileExist(const std::string registreName) override;
    bool createFile(const std::string fileName) override;
    bool readFrom(const std::string fileName, std::vector<std::string>& vect) override;
    bool addLine(const std::string fileName, const std::string line) override;
    bool clearFile(const std::string fileName) override;
};

/*FlashMem::FlashMem() {}// TODO : erase this line

FlashMem::~FlashMem() {}*/

#endif