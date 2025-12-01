#include "TicketService.h"
#include "Database.h"
#include "BaseVehicle.h"
#include <iostream>
#include <algorithm>

// convert string to uppercase for comparison
static std::string toUpper(std::string str)
{
    for (char &c : str)
    {
        if (c >= 'a' && c <= 'z')
        {
            c = c - 'a' + 'A';
        }
    }
    return str;
}

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
    // Simple fix: convert type to uppercase for storage
    TicketInfo upperTicket = ticket;
    upperTicket.type = toUpper(ticket.type);

    // Validate ticket information
    if (!validateTicketInfo(upperTicket))
    {
        std::cout << "Error: Invalid ticket information." << std::endl;
        return false;
    }

    // No duplicate check - multiple tickets of same type allowed (e.g., multiple Train tickets to different destinations)

    // Insert into database
    if (Database::getInstance().createTicket(upperTicket))
    {
        std::cout << "Ticket created successfully: " << upperTicket.type << " from " << upperTicket.origin << " to " << upperTicket.destination << std::endl;
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

    // Simple fix: convert type to uppercase
    TicketInfo upperInfo = newInfo;
    upperInfo.type = toUpper(newInfo.type);

    if (!validateTicketInfo(upperInfo))
    {
        std::cout << "Error: Invalid ticket information." << std::endl;
        return false;
    }

    if (Database::getInstance().updateTicketById(ticketId, upperInfo))
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
                  << " - From: " << tickets[i].origin << " To: " << tickets[i].destination
                  << " (Date: " << tickets[i].date
                  << ", Price: $" << tickets[i].price
                  << ", Available: " << tickets[i].availability << ")" << std::endl;
    }
}

bool TicketService::validateTicketInfo(const TicketInfo &ticket) const
{
    // Business rules for ticket validation
    if (ticket.type.empty() || ticket.origin.empty() || ticket.destination.empty())
    {
        std::cout << "Error: Type, origin, and destination cannot be empty." << std::endl;
        return false;
    }

    // Simple check: compare everything in uppercase
    std::string typeUpper = toUpper(ticket.type);
    if (typeUpper != "PLANE" && typeUpper != "CAB" && typeUpper != "TRAIN" && typeUpper != "BUS")
    {
        std::cout << "Error: Invalid ticket type '" << ticket.type << "'. Must be: PLANE, CAB, TRAIN, or BUS." << std::endl;
        return false;
    }

    if (ticket.price <= 0)
    {
        std::cout << "Error: Price must be greater than 0." << std::endl;
        return false;
    }

    if (ticket.availability < 0)
    {
        std::cout << "Error: Availability cannot be negative." << std::endl;
        return false;
    }

    if (ticket.date.empty())
    {
        std::cout << "Error: Date cannot be empty." << std::endl;
        return false;
    }

    return true;
}

bool TicketService::createTicketWithVehicle(const std::string &type, const std::string &origin, const std::string &destination,
                                            double price, int availability, const std::string &date,
                                            std::shared_ptr<BaseVehicle> vehicle)
{
    TicketInfo ticket;
    ticket.id = 0; // New ticket
    ticket.type = type;
    ticket.origin = origin;
    ticket.destination = destination;
    ticket.price = price;
    ticket.availability = availability;
    ticket.date = date;
    ticket.vehicleId = vehicle ? vehicle->getVehicleId() : 0;

    // Validate that the vehicle type matches the ticket type
    if (vehicle && vehicle->getVehicleType() != type)
    {
        std::cout << "Error: Vehicle type '" << vehicle->getVehicleType()
                  << "' does not match ticket type '" << type << "'." << std::endl;
        return false;
    }

    return createTicket(ticket);
}

std::shared_ptr<BaseVehicle> TicketService::getTicketVehicle(int ticketId) const
{
    // Get ticket info
    TicketInfo ticket = Database::getInstance().getTicketById(ticketId);

    if (ticket.id == 0 || ticket.vehicleId == 0)
    {
        return nullptr; // Ticket not found or not linked to a vehicle
    }

    // Get vehicle record and convert to object
    VehicleRecord vehicleRecord = Database::getInstance().getVehicleById(ticket.vehicleId);

    if (vehicleRecord.id == 0)
    {
        return nullptr; // Vehicle not found
    }

    return Database::getInstance().createVehicleObject(vehicleRecord);
}
