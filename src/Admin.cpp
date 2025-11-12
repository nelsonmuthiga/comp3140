#include "Admin.h"
#include <iostream>

// Parameterized constructor
Admin::Admin(int id, const std::string &userName, const std::string &password,
             const std::string &fullName, const std::string &email,
             const std::string &adminLevel)
    : BaseUser(id, userName, password, fullName, email), adminLevel(adminLevel) {}

void Admin::viewAllTickets() const
{
    std::cout << "--- All Tickets (Admin View) ---" << std::endl;
    std::cout << "All system bookings:" << std::endl;
    std::cout << "1. User: john_doe | Booking ID: BK1234 | Type: Plane | Tickets: 2" << std::endl;
    std::cout << "2. User: jane_smith | Booking ID: BK5678 | Type: Train | Tickets: 1" << std::endl;
    std::cout << "3. User: mike_jones | Booking ID: BK9012 | Type: Cab | Tickets: 4" << std::endl;
    // TODO: Implement admin ticket view from database
}

void Admin::manageBookings() const
{
    std::cout << "--- Manage Bookings ---" << std::endl;
    std::cout << "1. Add New Booking" << std::endl;
    std::cout << "2. Edit Booking" << std::endl;
    std::cout << "3. Delete Booking" << std::endl;
    std::cout << "4. Back" << std::endl;
    // TODO: Implement event management logic
}

void Admin::viewCustomerList() const
{
    std::cout << "--- Customer List ---" << std::endl;
    std::cout << "Registered customers:" << std::endl;
    std::cout << "Customer management feature coming soon..." << std::endl;
    // TODO: Implement customer list retrieval
}

void Admin::generateReports() const
{
    std::cout << "--- Generate Reports ---" << std::endl;
    std::cout << "Reporting feature coming soon..." << std::endl;
    // TODO: Implement report generation
}

// Getter
std::string Admin::getAdminLevel() const
{
    return adminLevel;
}

// Setter
void Admin::setAdminLevel(const std::string &level)
{
    this->adminLevel = level;
}
