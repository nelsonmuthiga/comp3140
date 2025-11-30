#ifndef TICKETSERVICE_H
#define TICKETSERVICE_H

#include <string>
#include <vector>

/**
 * Structure to hold ticket information
 */
struct TicketInfo
{
    int id;                  // Database ID (0 for new tickets)
    std::string type;        // Category: Cab, Plane, Train.
    std::string description; // Destination/route description
    double price;            // Price of the ticket
    int availability;        // Number of tickets available
    std::string date;        // Date of travel or availability (format: YYYY-MM-DD or descriptive text)
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
     * Get detailed information about a specific ticket by type (returns first match)
     * @param ticketType The type of ticket
     * @return TicketInfo structure with details
     */
    TicketInfo getTicketDetails(const std::string &ticketType) const;

    /**
     * Get detailed information about a specific ticket by ID
     * @param ticketId The database ID of the ticket
     * @return TicketInfo structure with details
     */
    TicketInfo getTicketById(int ticketId) const;

    /**
     * Update ticket availability (e.g., after booking or cancellation)
     * @param ticketId The database ID of the ticket
     * @param change The change in availability (negative for bookings, positive for cancellations)
     * @return true if update successful, false otherwise
     */
    bool updateTicketAvailability(int ticketId, int change);

    /**
     * Create a new ticket (admin function)
     * Multiple tickets of the same type are allowed (e.g., multiple Train tickets to different destinations)
     * @param ticket The ticket information to create
     * @return true if creation successful, false otherwise
     */
    bool createTicket(const TicketInfo &ticket);

    /**
     * Modify an existing ticket by ID (admin function)
     * @param ticketId The database ID of the ticket to modify
     * @param newInfo The new ticket information
     * @return true if modification successful, false otherwise
     */
    bool modifyTicket(int ticketId, const TicketInfo &newInfo);

    /**
     * Delete a ticket by ID (admin function)
     * @param ticketId The database ID of the ticket to delete
     * @return true if deletion successful, false otherwise
     */
    bool deleteTicket(int ticketId);

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
};

#endif
