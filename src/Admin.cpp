#include "Admin.h"

// Parameterized constructor
Admin::Admin(int id, const std::string &userName, const std::string &password,
             const std::string &fullName, const std::string &email)
    : BaseUser(id, userName, password, fullName, email) {}
