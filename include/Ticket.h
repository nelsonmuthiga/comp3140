#ifndef TICKET_H
#define TICKET_H

#include <string>
#include "BaseVehicle.h"
#include "BaseUser.h"

class Ticket
{
private:
    std::string ticketId;
    BaseUser *user;       // Pointer to the user who booked the ticket
    BaseVehicle *vehicle; // Pointer to the vehicle for which the ticket is booked
    std::string travelDate;
    std::string origin;
    std::string destination;
    double price;
    std::string status; // e.g., "booked", "canceled"
public:
    // default constructor
    Ticket() = default;
    // parameterized constructor
    Ticket(const std::string &ticketId, BaseUser *user, BaseVehicle *vehicle,
           const std::string &travelDate, const std::string &origin,
           const std::string &destination, double price, const std::string &status);

    void setStatus(const std::string &status);
    std::string getStatus() const;
    std::string getTicketId() const;
    bool isAvailable() const;
};

#endif