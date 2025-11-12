#include "Customer.h"
#include <iostream>
#include <algorithm>

// Parameterized constructor
Customer::Customer(int id, const std::string &userName, const std::string &password, 
                   const std::string &fullName, const std::string &email, 
                   const std::string &phone)
    : BaseUser(id, userName, password, fullName, email), phone(phone) {}

void Customer::bookTicket(const std::string &bookingId) {
    bookingIds.push_back(bookingId);
    std::cout << "Ticket booked successfully! Booking ID: " << bookingId << std::endl;
    // TODO: Implement actual booking logic with database
}

void Customer::viewMyTickets() const {
    std::cout << "--- My Tickets ---" << std::endl;
    if (bookingIds.empty()) {
        std::cout << "You have no tickets booked yet." << std::endl;
    } else {
        std::cout << "Your booked tickets:" << std::endl;
        for (size_t i = 0; i < bookingIds.size(); ++i) {
            std::cout << (i + 1) << ". Booking ID: " << bookingIds[i] << std::endl;
        }
    }
    // TODO: Retrieve actual ticket details from database
}

void Customer::cancelTicket(const std::string &bookingId) {
    auto it = std::find(bookingIds.begin(), bookingIds.end(), bookingId);
    if (it != bookingIds.end()) {
        bookingIds.erase(it);
        std::cout << "Ticket cancellation successful for Booking ID: " << bookingId << std::endl;
        std::cout << "Refund will be processed within 7 business days." << std::endl;
    } else {
        std::cout << "Booking ID not found: " << bookingId << std::endl;
    }
    // TODO: Implement actual cancellation logic with database
}

void Customer::browseEvents() const {
    std::cout << "--- Browse Available Tickets ---" << std::endl;
    std::cout << "Available Transportation Tickets:" << std::endl;
    std::cout << "1. Cab - City Taxi Service (Available: Now, Price: $25)" << std::endl;
    std::cout << "2. Plane - Flight to New York (Date: 2025-12-01, Price: $450)" << std::endl;
    std::cout << "3. Train - Express to Boston (Date: 2025-11-25, Price: $85)" << std::endl;
    // TODO: Retrieve actual tickets from database
}

// Getter
std::string Customer::getPhone() const {
    return phone;
}

// Setter
void Customer::setPhone(const std::string &phone) {
    this->phone = phone;
}

// Getter for bookings
std::vector<std::string> Customer::getBookingIds() const {
    return bookingIds;
}
