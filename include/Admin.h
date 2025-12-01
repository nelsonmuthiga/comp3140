#ifndef ADMIN_H
#define ADMIN_H

#include "BaseUser.h"
#include <string>

/**
 *Methods has been moved to service classes as per our instructors feedback on the last project submission  (AdminService, BookingService, TicketService and newly added VehicleService).
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
