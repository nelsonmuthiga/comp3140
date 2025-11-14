#include "TicketService.h"
#include <iostream>
#include <algorithm>

std::vector<TicketInfo> TicketService::getAvailableTickets() const
{
    std::vector<TicketInfo> tickets;

    // TODO: Retrieve from database
    // Placeholder data for demonstration
    tickets.push_back({"Cab", "City Taxi Service", 25.0, 50, "Available Now"});
    tickets.push_back({"Plane", "Flight to New York", 450.0, 20, "2025-12-01"});
    tickets.push_back({"Train", "Express to Boston", 85.0, 100, "2025-11-25"});

    return tickets;
}

TicketInfo TicketService::getTicketDetails(const std::string &ticketType) const
{
    // TODO: Retrieve specific ticket from database
    auto tickets = getAvailableTickets();

    for (const auto &ticket : tickets)
    {
        if (ticket.type == ticketType)
        {
            return ticket;
        }
    }

    // Return empty ticket if not found
    return {"", "", 0.0, 0, ""};
}

bool TicketService::updateTicketAvailability(const std::string &ticketType, int change)
{
    if (!ticketExists(ticketType))
    {
        std::cout << "Error: Ticket type does not exist." << std::endl;
        return false;
    }

    // NOTE: Cannot have negative availability
    // TODO: Update database with new availability
    std::cout << "Updated availability for " << ticketType << " by " << change << std::endl;

    return true;
}

bool TicketService::createTicket(const TicketInfo &ticket)
{
    // Validate ticket information
    if (!validateTicketInfo(ticket))
    {
        std::cout << "Error: Invalid ticket information." << std::endl;
        return false;
    }

    // Business rule: Check if ticket type already exists
    if (ticketExists(ticket.type))
    {
        std::cout << "Error: Ticket type already exists." << std::endl;
        return false;
    }

    // TODO: Insert into database
    std::cout << "Ticket created successfully: " << ticket.type << std::endl;

    return true;
}

bool TicketService::modifyTicket(const std::string &ticketType, const TicketInfo &newInfo)
{
    if (!ticketExists(ticketType))
    {
        std::cout << "Error: Ticket type does not exist." << std::endl;
        return false;
    }

    if (!validateTicketInfo(newInfo))
    {
        std::cout << "Error: Invalid ticket information." << std::endl;
        return false;
    }

    // TODO: Update database
    std::cout << "Ticket modified successfully: " << ticketType << std::endl;

    return true;
}

bool TicketService::deleteTicket(const std::string &ticketType)
{
    if (!ticketExists(ticketType))
    {
        std::cout << "Error: Ticket type does not exist." << std::endl;
        return false;
    }

    // NOTE: Check if there are active bookings for this ticket
    // TODO: Verify no active bookings before deletion

    // TODO: Delete from database
    std::cout << "Ticket deleted successfully: " << ticketType << std::endl;

    return true;
}

void TicketService::displayAvailableTickets() const
{
    auto tickets = getAvailableTickets();

    std::cout << "--- Available Transportation Tickets ---" << std::endl;
    if (tickets.empty())
    {
        std::cout << "No tickets available at this time." << std::endl;
        return;
    }

    for (size_t i = 0; i < tickets.size(); ++i)
    {
        std::cout << (i + 1) << ". " << tickets[i].type
                  << " - " << tickets[i].description
                  << " (Date: " << tickets[i].date
                  << ", Price: $" << tickets[i].price
                  << ", Available: " << tickets[i].availability << ")" << std::endl;
    }
}

bool TicketService::validateTicketInfo(const TicketInfo &ticket) const
{
    // Business rules for ticket validation
    if (ticket.type.empty() || ticket.description.empty())
    {
        return false;
    }

    if (ticket.price <= 0)
    {
        return false;
    }

    if (ticket.availability < 0)
    {
        return false;
    }

    return true;
}

bool TicketService::ticketExists(const std::string &ticketType) const
{
    // TODO: Check database
    // For now, check against hardcoded list
    auto tickets = getAvailableTickets();

    for (const auto &ticket : tickets)
    {
        if (ticket.type == ticketType)
        {
            return true;
        }
    }

    return false;
}
