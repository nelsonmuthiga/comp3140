#ifndef ADMIN_H
#define ADMIN_H

#include "BaseUser.h"
#include "Ticket.h"
#include "Vehicle.h"
#include <string>
#include <vector>

class Admin : public BaseUser
{
private:
    std::string adminLevel;
    std::vector<Ticket> tickets;
    std::vector<Vehicle> vehicles;
    
public:
    Admin() = default;

    Admin(int id, const std::string &userName, const std::string &password, 
          const std::string &fullName, const std::string &email, 
          const std::string &adminLevel = "regular");

    void addTicket();
    void editTicket();
    void deleteTicket();
    void viewAllTickets() const;
    void manageBookings(); // REMOVED const
    void viewCustomerList() const;
    void generateReports() const;
    
    std::string getAdminLevel() const;
    void setAdminLevel(const std::string &level);
};

#endif