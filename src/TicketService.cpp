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
    // Retrieve specific ticket from database (returns first match)
    return Database::getInstance().getTicketByType(ticketType);
}

TicketInfo TicketService::getTicketById(int ticketId) const
{
    return Database::getInstance().getTicketById(ticketId);
}

bool TicketService::updateTicketAvailability(int ticketId, int change)
{
    TicketInfo ticket = Database::getInstance().getTicketById(ticketId);
    if (ticket.id == 0)
    {
        std::cout << "Error: Ticket not found." << std::endl;
        return false;
    }

    // Update database with new availability
    if (Database::getInstance().updateTicketAvailability(ticketId, change))
    {
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

    // No duplicate check - multiple tickets of same type allowed (e.g., Train to Boston, Train to Chicago)

    // Insert into database
    if (Database::getInstance().createTicket(ticket))
    {
        std::cout << "Ticket created successfully: " << ticket.type << " - " << ticket.description << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: Failed to create ticket in database." << std::endl;
        return false;
    }
}

bool TicketService::modifyTicket(int ticketId, const TicketInfo &newInfo)
{
    TicketInfo existing = Database::getInstance().getTicketById(ticketId);
    if (existing.id == 0)
    {
        std::cout << "Error: Ticket not found." << std::endl;
        return false;
    }

    if (!validateTicketInfo(newInfo))
    {
        std::cout << "Error: Invalid ticket information." << std::endl;
        return false;
    }

    if (Database::getInstance().updateTicketById(ticketId, newInfo))
    {
        std::cout << "Ticket modified successfully!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Error: Failed to modify ticket in database." << std::endl;
        return false;
    }
}

bool TicketService::deleteTicket(int ticketId)
{
    TicketInfo existing = Database::getInstance().getTicketById(ticketId);
    if (existing.id == 0)
    {
        std::cout << "Error: Ticket not found." << std::endl;
        return false;
    }

    // Delete from database
    if (Database::getInstance().deleteTicketById(ticketId))
    {
        std::cout << "Ticket deleted successfully!" << std::endl;
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
        std::cout << (i + 1) << ". [ID:" << tickets[i].id << "] " << tickets[i].type
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
