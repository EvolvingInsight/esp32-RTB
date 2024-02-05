/**************************************************************************************
Program :   User.hpp
Author :    Loïc Bouillon
Date :      01/06/2022
Purpose :   User class that holds every data needed for verification and access rights
**************************************************************************************/
#pragma once
#include <string>

using namespace std;

class User {
private:
    char _letterIdentifier;     // Identify the user
    string _password;           // User's password
    int _tokens;                // How often the user is allowed to access content
    int _usedTokens;            // How often the user has accessed content

public:
    User();
    User(const User &u);// = default
    ~User();

    void init(const char let, const string pwd, int tokens, int usedTokens = 0);

    void resetUsedTokens();     // Reset to zero _usedTokens
    void addUsedTokens();       // Increment value of _usedTokens
    
    char getLetter()const;      // Return value of _letterIdentifier
    string getPwd()const;       // Return value of _password
    int getTokens() const;      // Return value of _tokens
    int getUsedTokens() const;  // Return value of _usedTokens
    bool isAllowed() const;     // Tells if user is allow to access content
};

User::User() {}
User::User(const User &u){
    this->_letterIdentifier = u.getLetter();
    this->_password = u.getPwd();
    this->_tokens = u.getTokens();
    this->_usedTokens = u.getUsedTokens();

}
User::~User() {}

void User::init(const char let, const string pwd, int tokens, int usedTokens) {
    _letterIdentifier = let;
    _password = pwd;
    _tokens = tokens;
    _usedTokens = usedTokens;
}

char User::getLetter()const{
    return _letterIdentifier;
}

string User::getPwd()const{
    return _password;
}

void User::resetUsedTokens(){
    _usedTokens = 0;
}

void User::addUsedTokens(){
    _usedTokens += 1;
}

int User::getTokens() const{
    return _tokens;
}
int User::getUsedTokens() const{
    return _usedTokens;
}

bool User::isAllowed() const{
    return _usedTokens < _tokens;
}