#ifndef ADMIN_H
#define ADMIN_H

#include "BaseUser.h"
#include <string>

class Admin : public BaseUser
{
private:
    std::string adminLevel; // e.g., "super", "regular"

public:
    // default constructor
    Admin() = default;

    // parameterized constructor
    Admin(int id, const std::string &userName, const std::string &password,
          const std::string &fullName, const std::string &email,
          const std::string &adminLevel = "regular");

    // Admin-specific methods
    void viewAllTickets() const;
    void manageBookings() const;
    void viewCustomerList() const;
    void generateReports() const;

    // getter and setter for adminLevel
    std::string getAdminLevel() const;
    void setAdminLevel(const std::string &level);
};

#endif
