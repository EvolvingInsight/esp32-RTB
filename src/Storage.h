/**
 * File :      Storage.h
 * Author :    Loïc Bouillon
 * Date :      03/06/2022
 * Purpose :   Storage class "wrapper". It allows the manipulation of files through child class.
*/
#ifndef STORAGE_H
#define STORAGE_H

#include <string>	// Library for string
#include <vector>	// Library for vector
#include "DEFINITIONS.hpp"

class Storage {
    
public:
    virtual ~Storage() = default;

    virtual bool init() = 0;
    
    virtual bool fileExist(const std::string registreName) = 0;
    virtual bool createFile(const std::string fileName) = 0;
    virtual bool readFrom(const std::string fileName, std::vector<std::string>& vect) = 0;
    virtual bool addLine(const std::string fileName, const std::string line) = 0;
    virtual bool clearFile(const std::string fileName) = 0;
};

#endif