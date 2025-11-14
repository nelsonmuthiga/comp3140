#ifndef ADMIN_H
#define ADMIN_H

#include "BaseUser.h"
#include <string>

/**
 * Admin entity class - maintains only internal state and identity.
 * Business logic has been moved to service classes (AdminService, BookingService, TicketService).
 */
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

    // State management methods only - no business logic
    std::string getAdminLevel() const;
    void setAdminLevel(const std::string &level);
};

#endif
