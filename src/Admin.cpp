#include "Admin.h"

// Parameterized constructor
Admin::Admin(int id, const std::string &userName, const std::string &password,
             const std::string &fullName, const std::string &email,
             const std::string &adminLevel)
    : BaseUser(id, userName, password, fullName, email), adminLevel(adminLevel) {}

// Simple state management - no business logic
std::string Admin::getAdminLevel() const
{
    return adminLevel;
}

void Admin::setAdminLevel(const std::string &level)
{
    this->adminLevel = level;
}
