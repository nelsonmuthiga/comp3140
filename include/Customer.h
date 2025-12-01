#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "BaseUser.h"
#include <string>
#include <vector>

/**
 * Customer entity class - maintains only internal state and identity.
 * Business logic has been moved to service classes (BookingService, TicketService).
 */
class Customer : public BaseUser
{
private:
    std::string phone;
    std::vector<std::string> bookingIds; // Store booking IDs

public:
    // default constructor
    Customer() = default;

    // parameterized constructor
    Customer(int id, const std::string &userName, const std::string &password,
             const std::string &fullName, const std::string &email,
             const std::string &phone);

    // getters
    std::string getPhone() const;
    void setPhone(const std::string &phone);

    std::vector<std::string> getBookingIds() const;
    void addBookingId(const std::string &bookingId);
    void removeBookingId(const std::string &bookingId);
};

#endif
