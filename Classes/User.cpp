#include "User.h"
#include <iostream>

using namespace std;

User::User() : username(""), password(""), fullname(""), email("") {}

User::User(const string& uname, const string& pwd, const string& fname, const string& eml)
    : username(uname), password(pwd), fullname(fname), email(eml)
{
}

User::User(const User& other)
    : username(other.username), password(other.password), fullname(other.fullname), email(other.email)
{
}

void User::setUsername(const string& uname) {
    username = uname;
}

void User::setPassword(const string& pwd) {
    password = pwd;
}

void User::setFullname(const string& fname) {
    fullname = fname;
}

void User::setEmail(const string& eml) {
    email = eml;
}

string User::getUsername() const {
    return username;
}

string User::getPassword() const {
    return password;
}

string User::getFullname() const {
    return fullname;
}

string User::getEmail() const {
    return email;
}

void User::addUserToDatabase() {
    // Implementation for adding user to database goes here
}

bool User::checkUsernameAndPassword(const string& uname, const string& pwd) const {
    // Check if the provided username and password match the stored ones in Database
}

User::~User() {
    // Destructor
}