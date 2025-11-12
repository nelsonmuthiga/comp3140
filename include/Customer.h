#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "BaseUser.h"
#include <string>
#include <vector>

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

    // Customer-specific methods
    void bookTicket(const std::string &bookingId);
    void viewMyTickets() const;
    void cancelTicket(const std::string &bookingId);
    void browseEvents() const;
    
    // getter and setter for phone
    std::string getPhone() const;
    void setPhone(const std::string &phone);
    
    // getter for bookings
    std::vector<std::string> getBookingIds() const;
};

#endif
