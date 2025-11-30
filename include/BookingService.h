#ifndef BOOKINGSERVICE_H
#define BOOKINGSERVICE_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
class Customer;

/**
 * BookingService handles all business logic related to ticket bookings.
 * This includes creating bookings, cancellations, and managing booking state.
 */
class BookingService
{
public:
    BookingService() = default;

    /**
     * Create a new booking for a customer using ticket ID
     * @param customer The customer making the booking
     * @param ticketId Database ID of the ticket being booked
     * @param numTickets Number of tickets to book
     * @return Booking ID if successful, empty string if failed
     */
    std::string createBooking(std::shared_ptr<Customer> customer,
                              int ticketId,
                              int numTickets);

    /**
     * Cancel an existing booking
     * @param bookingId The booking ID to cancel
     * @param customer The customer who owns the booking
     * @return true if cancellation successful, false otherwise
     */
    bool cancelBooking(const std::string &bookingId,
                       std::shared_ptr<Customer> customer);

    /**
     * Get all bookings for a specific customer
     * @param customerId The customer's ID
     * @return Vector of booking details as strings
     */
    std::vector<std::string> getCustomerBookings(int customerId) const;

    /**
     * Validate if a booking exists and is valid
     * @param bookingId The booking ID to validate
     * @return true if booking is valid, false otherwise
     */
    bool validateBooking(const std::string &bookingId) const;

    /**
     * Get all bookings in the system (admin function)
     * @return Vector of all booking details
     */
    std::vector<std::string> getAllBookings() const;

private:
    /**
     * Check if booking can be cancelled (based on business rules)
     * @param bookingId The booking ID
     * @return true if cancellation is allowed, false otherwise
     */
    bool canCancelBooking(const std::string &bookingId) const;

    /**
     * Calculate refund amount for a cancellation
     * @param bookingId The booking ID
     * @return Refund amount
     */
    double calculateRefund(const std::string &bookingId) const;

    /**
     * Generate a unique booking ID
     * @return Unique booking ID string
     */
    std::string generateBookingId() const;
};

#endif
