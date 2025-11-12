#include "Admin.h"
#include <iostream>
#include <algorithm>  // For std::remove_if
#include <chrono>
#include <string>
#include <vector>

using namespace std;

// Parameterized constructor
Admin::Admin(int id, const std::string &userName, const std::string &password, 
             const std::string &fullName, const std::string &email, 
             const std::string &adminLevel)
    : BaseUser(id, userName, password, fullName, email), adminLevel(adminLevel) {}

void Admin::addTicket() {
    string ticketId, status, origin, destination;
    string vehicleId, vehicleType;
    int vehicleCapacity;
    float price;
    
    cout << "--- Add Ticket ---" << endl;
    cout << "Enter ticket details:" << endl;
    cout << "Ticket ID: ";
    cin >> ticketId;
    cout << "Status: ";
    cin >> status;
    cout << "Price: ";
    cin >> price;
    cout << "Vehicle ID: ";
    cin >> vehicleId;
    cout << "Vehicle Type: ";
    cin >> vehicleType;
    cout << "Vehicle Capacity: ";
    cin >> vehicleCapacity;
    cout << "Origin: ";
    cin >> origin;
    cout << "Destination: ";
    cin >> destination;
    
    // Create vehicle
    Vehicle vehicle(vehicleId, vehicleType, vehicleCapacity);
    
    // Use current time for date
    auto date = chrono::system_clock::now();
    
    // Create a default BaseUser (or get from system)
    BaseUser user;
    
    // Create ticket
    Ticket ticket(ticketId, status, user, date, price, vehicle, origin, destination);
    tickets.push_back(ticket);
    
    cout << "Ticket added successfully!" << endl;
}

void Admin::editTicket() {
    string ticketId, status, origin, destination;
    string vehicleId, vehicleType;
    int vehicleCapacity;
    float price;

    cout << "--- Edit Ticket ---" << endl;
    cout << "Enter ticket ID to edit: ";
    cin >> ticketId;
    
    // TODO: Find and edit existing ticket
    cout << "Enter new ticket details:" << endl;
    cout << "Status: ";
    cin >> status;
    cout << "Price: ";
    cin >> price;
    cout << "Vehicle ID: ";
    cin >> vehicleId;
    cout << "Vehicle Type: ";
    cin >> vehicleType;
    cout << "Vehicle Capacity: ";
    cin >> vehicleCapacity;
    cout << "Origin: ";
    cin >> origin;
    cout << "Destination: ";
    cin >> destination;
    
    Vehicle vehicle(vehicleId, vehicleType, vehicleCapacity);
    auto date = chrono::system_clock::now();
    BaseUser user;
    
    Ticket ticket(ticketId, status, user, date, price, vehicle, origin, destination);
    tickets.push_back(ticket);
    
    cout << "Ticket edited successfully!" << endl;
}

void Admin::deleteTicket() {
    string ticketId;
    cout << "--- Delete Ticket ---" << endl;
    cout << "Enter ticket ID to delete: ";
    cin >> ticketId;
    
    tickets.erase(
        remove_if(tickets.begin(), tickets.end(), 
            [&ticketId](const Ticket& ticket) { 
                return ticket.getTicketId() == ticketId; 
            }
        ), 
        tickets.end()
    );
    
    cout << "Ticket deleted successfully!" << endl;
}

void Admin::viewAllTickets() const {
    cout << "--- All Tickets (Admin View) ---" << endl;
    cout << "All system bookings:" << endl;
    for (const auto& ticket : tickets) {
        cout << "Ticket ID: " << ticket.getTicketId() 
             << " | Status: " << ticket.getStatus()
             << " | Price: $" << ticket.getPrice() << endl;
    }
}

void Admin::manageBookings() { // REMOVED const
    cout << "--- Manage Bookings ---" << endl;
    cout << "1. Add New Booking" << endl;
    cout << "2. Edit Booking" << endl;
    cout << "3. Delete Booking" << endl;
    cout << "4. Back" << endl;
    
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    
    switch (choice) {
        case 1:
            addTicket();
            break;
        case 2:
            editTicket();
            break;
        case 3:
            deleteTicket();
            break;
        case 4:
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
    }
}

void Admin::viewCustomerList() const {
    cout << "--- Customer List ---" << endl;
    cout << "Registered customers:" << endl;
    cout << "Customer management feature coming soon..." << endl;
}

void Admin::generateReports() const {
    cout << "--- Generate Reports ---" << endl;
    cout << "Reporting feature coming soon..." << endl;
}

string Admin::getAdminLevel() const {
    return adminLevel;
}

void Admin::setAdminLevel(const string &level) {
    this->adminLevel = level;
}