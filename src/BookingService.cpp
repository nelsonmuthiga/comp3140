#include "BookingService.h"
#include "Customer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

std::string BookingService::createBooking(std::shared_ptr<Customer> customer,
                                          const std::string &ticketType,
                                          int numTickets)
{
    // Validate inputs
    if (!customer || numTickets <= 0)
    {
        std::cout << "Error: Invalid booking parameters." << std::endl;
        return "";
    }

    // Business rule: Check ticket availability
    if (!isTicketAvailable(ticketType, numTickets))
    {
        std::cout << "Error: Insufficient tickets available." << std::endl;
        return "";
    }

    // Generate unique booking ID
    std::string bookingId = generateBookingId();

    // Business logic: Process booking
    std::cout << "Processing booking..." << std::endl;
    std::cout << "Ticket Type: " << ticketType << std::endl;
    std::cout << "Number of Tickets: " << numTickets << std::endl;
    std::cout << "Booking ID: " << bookingId << std::endl;

    // TODO: Update database with booking information
    // TODO: Process payment
    // TODO: Update ticket availability

    std::cout << "Booking created successfully!" << std::endl;

    return bookingId;
}

bool BookingService::cancelBooking(const std::string &bookingId,
                                   std::shared_ptr<Customer> customer)
{
    if (!customer || bookingId.empty())
    {
        std::cout << "Error: Invalid cancellation parameters." << std::endl;
        return false;
    }

    // Business rule: Validate booking ownership
    auto bookings = customer->getBookingIds();
    if (std::find(bookings.begin(), bookings.end(), bookingId) == bookings.end())
    {
        std::cout << "Error: Booking ID not found for this customer." << std::endl;
        return false;
    }

    // Business rule: Check if cancellation is allowed
    if (!canCancelBooking(bookingId))
    {
        std::cout << "Error: This booking cannot be cancelled at this time." << std::endl;
        return false;
    }

    // Calculate refund
    double refundAmount = calculateRefund(bookingId);

    std::cout << "Cancelling booking: " << bookingId << std::endl;
    std::cout << "Refund amount: $" << refundAmount << std::endl;
    std::cout << "Refund will be processed within 7 business days." << std::endl;

    // TODO: Update database
    // TODO: Process refund
    // TODO: Update ticket availability

    return true;
}

std::vector<std::string> BookingService::getCustomerBookings(int customerId) const
{
    std::vector<std::string> bookings;

    // TODO: Retrieve from database
    // For now, return placeholder data
    bookings.push_back("Booking ID: BK1001 | Type: Cab | Tickets: 1 | Status: Active");
    bookings.push_back("Booking ID: BK1002 | Type: Train | Tickets: 2 | Status: Active");

    return bookings;
}

bool BookingService::validateBooking(const std::string &bookingId) const
{
    if (bookingId.empty())
    {
        return false;
    }

    // TODO: Check database for booking validity
    // For now, simple validation
    return bookingId.length() >= 4 && bookingId.substr(0, 2) == "BK";
}

std::vector<std::string> BookingService::getAllBookings() const
{
    std::vector<std::string> allBookings;

    // TODO: Retrieve all bookings from database
    // Placeholder data for demonstration
    allBookings.push_back("User: john_doe | Booking ID: BK1234 | Type: Plane | Tickets: 2 | Status: Active");
    allBookings.push_back("User: jane_smith | Booking ID: BK5678 | Type: Train | Tickets: 1 | Status: Active");
    allBookings.push_back("User: mike_jones | Booking ID: BK9012 | Type: Cab | Tickets: 4 | Status: Cancelled");

    return allBookings;
}

bool BookingService::isTicketAvailable(const std::string &ticketType, int quantity) const
{
    // TODO: Check actual availability from database
    // Business rule: Validate ticket type and quantity
    if (quantity <= 0 || quantity > 10)
    {
        return false;
    }

    // For now, assume all types are available with limited quantity
    return !ticketType.empty();
}

bool BookingService::canCancelBooking(const std::string &bookingId) const
{
    // Business rule: Check cancellation policy
    // TODO: Check booking date, time, and cancellation policy
    // For now, allow all cancellations
    return validateBooking(bookingId);
}

double BookingService::calculateRefund(const std::string &bookingId) const
{
    // Business rule: Refund calculation based on cancellation policy
    // TODO: Implement actual refund calculation based on booking details

    // Placeholder: 80% refund for demo purposes
    double bookingAmount = 100.0; // TODO: Get from database
    double refundPercentage = 0.80;

    return bookingAmount * refundPercentage;
}

std::string BookingService::generateBookingId() const
{
    // Generate unique booking ID
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    int randomNum = std::rand() % 10000 + 1000;
    return "BK" + std::to_string(randomNum);
}
