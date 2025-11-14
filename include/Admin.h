#ifndef ADMIN_H
#define ADMIN_H

#include "BaseUser.h"
#include <string>

/**
 * Admin entity class - maintains only internal state and identity.
 * Business logic has been moved to service classes as per our instructors feedback on the last project submission  (AdminService, BookingService, TicketService).
 */
class Admin : public BaseUser
{
public:
    // default constructor
    Admin() = default;

    // parameterized constructor
    Admin(int id, const std::string &userName, const std::string &password,
          const std::string &fullName, const std::string &email);
};

#endif
