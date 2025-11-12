#include "BaseUser.h"
#include <iostream>

// Parameterized constructor
BaseUser::BaseUser(int id, const std::string &userName, const std::string &password,
                   const std::string &fullName, const std::string &email)
    : id(id), userName(userName), password(password), fullName(fullName), email(email) {}

void BaseUser::updateInfo()
{
    std::cout << "Updating user information for: " << userName << std::endl;
    // TODO: Implement update logic
}

bool BaseUser::login(const std::string &userName, const std::string &password)
{
    if (this->userName == userName && this->password == password)
    {
        std::cout << "Login successful for user: " << userName << std::endl;
        return true;
    }
    std::cout << "Login failed for user: " << userName << std::endl;
    return false;
}

bool BaseUser::logout()
{
    std::cout << "User " << userName << " logged out successfully." << std::endl;
    return true;
}

// Getters
int BaseUser::getId() const
{
    return id;
}

std::string BaseUser::getUserName() const
{
    return userName;
}

std::string BaseUser::getPassword() const
{
    return password;
}

std::string BaseUser::getFullName() const
{
    return fullName;
}

std::string BaseUser::getEmail() const
{
    return email;
}

// Setters
void BaseUser::setUserName(const std::string &userName)
{
    this->userName = userName;
}

void BaseUser::setPassword(const std::string &password)
{
    this->password = password;
}

void BaseUser::setFullName(const std::string &fullName)
{
    this->fullName = fullName;
}

void BaseUser::setEmail(const std::string &email)
{
    this->email = email;
}

void BaseUser::setId(int id)
{
    this->id = id;
}
