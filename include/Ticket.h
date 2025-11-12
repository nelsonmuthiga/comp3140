#ifndef TICKET_H
#define TICKET_H
#include <string>
#include <chrono>
#include "BaseUser.h"
#include "Vehicle.h"

class Ticket {

public:
    Ticket(); // default constructor

    // Fixed: added & after BaseUser and Vehicle, added std:: before chrono
    Ticket(const std::string&, const std::string&, const BaseUser&, 
           const std::chrono::system_clock::time_point&, float, 
           const Vehicle&, const std::string&, const std::string&);

    void setTicketId(const std::string&);
    void setStatus(const std::string&);
    void setUser(const BaseUser&);
    void setDate(const std::chrono::system_clock::time_point&);
    void setPrice(float);
    void setVehicle(const Vehicle&);
    void setOrigin(const std::string&);
    void setDestination(const std::string&);

    std::string getTicketId() const;
    std::string getStatus() const;
    BaseUser getUser() const;
    std::chrono::system_clock::time_point getDate() const; // Changed from date::date
    float getPrice() const;
    Vehicle getVehicle() const;
    std::string getOrigin() const;
    std::string getDestination() const;

    ~Ticket();

private:
    std::string ticketId;
    std::string status;
    BaseUser user;
    std::chrono::system_clock::time_point date; // Changed from date::date
    float price;
    Vehicle vehicle;
    std::string origin;
    std::string destination;
};
#endif