#include "BookingService.h"
#include "Customer.h"
#include "Database.h"
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

    // Business rule: Check ticket availability from database
    TicketInfo ticket = Database::getInstance().getTicketByType(ticketType);
    if (ticket.type.empty() || ticket.availability < numTickets)
    {
        std::cout << "Error: Insufficient tickets available." << std::endl;
        return "";
    }

    // Get ticket ID for booking
    int ticketId = Database::getInstance().getTicketIdByType(ticketType);
    if (ticketId < 0)
    {
        std::cout << "Error: Ticket type not found." << std::endl;
        return "";
    }

    // Calculate total price
    double totalPrice = ticket.price * numTickets;

    // Create booking in database
    std::string bookingId = Database::getInstance().createBooking(
        customer->getId(), ticketId, ticketType, numTickets, totalPrice);

    if (bookingId.empty())
    {
        std::cout << "Error: Failed to create booking." << std::endl;
        return "";
    }

    // Update ticket availability (decrease by numTickets)
    Database::getInstance().updateTicketAvailability(ticketType, -numTickets);

    // Display booking confirmation
    std::cout << "Processing booking..." << std::endl;
    std::cout << "Ticket Type: " << ticketType << std::endl;
    std::cout << "Number of Tickets: " << numTickets << std::endl;
    std::cout << "Total Price: $" << totalPrice << std::endl;
    std::cout << "Booking ID: " << bookingId << std::endl;
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

    // Get booking from database to validate ownership
    BookingRecord booking = Database::getInstance().getBookingById(bookingId);
    if (booking.bookingId.empty())
    {
        std::cout << "Error: Booking not found." << std::endl;
        return false;
    }

    // Validate booking belongs to this customer
    if (booking.userId != customer->getId())
    {
        std::cout << "Error: Booking ID not found for this customer." << std::endl;
        return false;
    }

    // Check if already cancelled
    if (booking.status == "cancelled")
    {
        std::cout << "Error: Booking is already cancelled." << std::endl;
        return false;
    }

    // Calculate refund (80% of total price)
    double refundAmount = booking.totalPrice * 0.80;

    // Cancel booking in database
    if (!Database::getInstance().cancelBooking(bookingId))
    {
        std::cout << "Error: Failed to cancel booking." << std::endl;
        return false;
    }

    // Restore ticket availability
    Database::getInstance().updateTicketAvailability(booking.ticketType, booking.numTickets);

    std::cout << "Cancelling booking: " << bookingId << std::endl;
    std::cout << "Refund amount: $" << refundAmount << std::endl;
    std::cout << "Refund will be processed within 7 business days." << std::endl;

    return true;
}

std::vector<std::string> BookingService::getCustomerBookings(int customerId) const
{
    std::vector<std::string> bookings;

    // Retrieve from database
    auto bookingRecords = Database::getInstance().getBookingsByUserId(customerId);

    for (const auto &record : bookingRecords)
    {
        std::string bookingStr = "Booking ID: " + record.bookingId +
                                 " | Type: " + record.ticketType +
                                 " | Tickets: " + std::to_string(record.numTickets) +
                                 " | Price: $" + std::to_string(record.totalPrice) +
                                 " | Status: " + record.status;
        bookings.push_back(bookingStr);
    }

    return bookings;
}

bool BookingService::validateBooking(const std::string &bookingId) const
{
    if (bookingId.empty())
    {
        return false;
    }

    // Check database for booking validity
    return Database::getInstance().bookingExists(bookingId);
}

std::vector<std::string> BookingService::getAllBookings() const
{
    std::vector<std::string> allBookings;

    // Retrieve all bookings from database
    auto bookingRecords = Database::getInstance().getAllBookings();

    for (const auto &record : bookingRecords)
    {
        std::string bookingStr = "User: " + record.userName +
                                 " | Booking ID: " + record.bookingId +
                                 " | Type: " + record.ticketType +
                                 " | Tickets: " + std::to_string(record.numTickets) +
                                 " | Price: $" + std::to_string(record.totalPrice) +
                                 " | Status: " + record.status;
        allBookings.push_back(bookingStr);
    }

    return allBookings;
}

bool BookingService::isTicketAvailable(const std::string &ticketType, int quantity) const
{
    // Business rule: Validate quantity
    if (quantity <= 0 || quantity > 10)
    {
        return false;
    }

    // Check actual availability from database
    TicketInfo ticket = Database::getInstance().getTicketByType(ticketType);
    return !ticket.type.empty() && ticket.availability >= quantity;
}

bool BookingService::canCancelBooking(const std::string &bookingId) const
{
    // Check if booking exists and is active
    BookingRecord booking = Database::getInstance().getBookingById(bookingId);
    return !booking.bookingId.empty() && booking.status == "active";
}

double BookingService::calculateRefund(const std::string &bookingId) const
{
    // Get booking from database
    BookingRecord booking = Database::getInstance().getBookingById(bookingId);

    if (booking.bookingId.empty())
    {
        return 0.0;
    }

    // Business rule: 80% refund
    double refundPercentage = 0.80;
    return booking.totalPrice * refundPercentage;
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
