#include "Storage.h"

#ifndef NULLSTORAGE_H
#define NULLSTORAGE_H

/**
 * Child class of Storage
*/
class NullStorage : public Storage{
public:
    NullStorage();                                // Constructor & destructor
    NullStorage(const NullStorage &u) = delete;   // Deletion of copy constructor, security for assuring there's only one instance

    ~NullStorage();
    
    bool init() override {return true;};

    bool fileExist(const std::string registreName) override {return true;};
    bool createFile(const std::string fileName) override {return true;};
    bool readFrom(const std::string fileName, std::vector<std::string>& vect) override {return true;};
    bool addLine(const std::string fileName, const std::string line) override {return true;};
    bool clearFile(const std::string fileName) override {return true;};
};

NullStorage::NullStorage() {}

NullStorage::~NullStorage() {}

#endif