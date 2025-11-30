#include "TicketService.h"
#include "Database.h"
#include <iostream>
#include <algorithm>

std::vector<TicketInfo> TicketService::getAvailableTickets() const
{
    // Retrieve tickets from database
    return Database::getInstance().getAllTickets();
}

TicketInfo TicketService::getTicketDetails(const std::string &ticketType) const
{
    // Retrieve specific ticket from database
    return Database::getInstance().getTicketByType(ticketType);
}

bool TicketService::updateTicketAvailability(const std::string &ticketType, int change)
{
    if (!ticketExists(ticketType))
    {
        std::cout << "Error: Ticket type does not exist." << std::endl;
        return false;
    }

    // Update database with new availability
    if (Database::getInstance().updateTicketAvailability(ticketType, change))
    {
        std::cout << "Updated availability for " << ticketType << " by " << change << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: Failed to update availability (insufficient tickets)." << std::endl;
        return false;
    }
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

    // Insert into database
    if (Database::getInstance().createTicket(ticket))
    {
        std::cout << "Ticket created successfully: " << ticket.type << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: Failed to create ticket in database." << std::endl;
        return false;
    }
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

    // Delete old ticket and create new one (simple update approach)
    if (Database::getInstance().deleteTicket(ticketType) && Database::getInstance().createTicket(newInfo))
    {
        std::cout << "Ticket modified successfully: " << ticketType << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: Failed to modify ticket in database." << std::endl;
        return false;
    }
}

bool TicketService::deleteTicket(const std::string &ticketType)
{
    if (!ticketExists(ticketType))
    {
        std::cout << "Error: Ticket type does not exist." << std::endl;
        return false;
    }

    // Delete from database
    if (Database::getInstance().deleteTicket(ticketType))
    {
        std::cout << "Ticket deleted successfully: " << ticketType << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: Failed to delete ticket from database." << std::endl;
        return false;
    }
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
    // Check database for ticket existence
    return Database::getInstance().ticketExists(ticketType);
}
