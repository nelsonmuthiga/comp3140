#ifndef TICKETSERVICE_H
#define TICKETSERVICE_H

#include <string>
#include <vector>

/**
 * Structure to hold ticket information
 */
struct TicketInfo
{
    std::string type;
    std::string description;
    double price;
    int availability;
    std::string date;
};

/**
 * TicketService handles all business logic related to ticket management.
 * This includes browsing tickets, managing ticket inventory, and ticket CRUD operations.
 */
class TicketService
{
public:
    TicketService() = default;

    /**
     * Get all available tickets in the system
     * @return Vector of available ticket information
     */
    std::vector<TicketInfo> getAvailableTickets() const;

    /**
     * Get detailed information about a specific ticket type
     * @param ticketType The type of ticket
     * @return TicketInfo structure with details
     */
    TicketInfo getTicketDetails(const std::string &ticketType) const;

    /**
     * Update ticket availability (e.g., after booking or cancellation)
     * @param ticketType The type of ticket
     * @param change The change in availability (negative for bookings, positive for cancellations)
     * @return true if update successful, false otherwise
     */
    bool updateTicketAvailability(const std::string &ticketType, int change);

    /**
     * Create a new ticket type (admin function)
     * @param ticket The ticket information to create
     * @return true if creation successful, false otherwise
     */
    bool createTicket(const TicketInfo &ticket);

    /**
     * Modify an existing ticket type (admin function)
     * @param ticketType The type of ticket to modify
     * @param newInfo The new ticket information
     * @return true if modification successful, false otherwise
     */
    bool modifyTicket(const std::string &ticketType, const TicketInfo &newInfo);

    /**
     * Delete a ticket type (admin function)
     * @param ticketType The type of ticket to delete
     * @return true if deletion successful, false otherwise
     */
    bool deleteTicket(const std::string &ticketType);

    /**
     * Display all available tickets (for browsing)
     */
    void displayAvailableTickets() const;

private:
    /**
     * Validate ticket information before creation or modification
     * @param ticket The ticket information to validate
     * @return true if valid, false otherwise
     */
    bool validateTicketInfo(const TicketInfo &ticket) const;

    /**
     * Check if a ticket type exists
     * @param ticketType The type of ticket
     * @return true if exists, false otherwise
     */
    bool ticketExists(const std::string &ticketType) const;
};

#endif
