#include "AdminService.h"
#include "Admin.h"
#include "Database.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <vector>
#include <utility>

std::vector<std::string> AdminService::viewAllCustomers() const
{
    std::vector<std::string> customers;

    // Retrieve from database
    auto customerRecords = Database::getInstance().getAllCustomers();

    for (const auto &record : customerRecords)
    {
        std::string customerStr = "ID: " + std::to_string(record.id) +
                                  " | Name: " + record.fullName +
                                  " | Email: " + record.email +
                                  " | Phone: " + record.phone;
        customers.push_back(customerStr);
    }

    return customers;
}

std::string AdminService::generateSalesReport() const
{
    std::ostringstream report;

    report << "\n========================================\n";
    report << "         SALES REPORT\n";
    report << "========================================\n\n";

    // Get data from database
    double totalRevenue = Database::getInstance().getTotalRevenue();
    int totalBookings = Database::getInstance().getTotalBookings();
    int activeBookings = Database::getInstance().getActiveBookings();
    int cancelledBookings = Database::getInstance().getCancelledBookings();

    report << std::fixed << std::setprecision(2);
    report << "Total Revenue:       $" << totalRevenue << "\n";
    report << "Total Bookings:      " << totalBookings << "\n";
    report << "Active Bookings:     " << activeBookings << "\n";
    report << "Cancelled Bookings:  " << cancelledBookings << "\n\n";

    // Get ticket breakdown from all bookings
    auto allBookings = Database::getInstance().getAllBookings();
    std::map<std::string, std::pair<int, double>> ticketBreakdown;

    for (const auto &booking : allBookings)
    {
        if (booking.status == "active")
        {
            ticketBreakdown[booking.ticketType].first += booking.numTickets;
            ticketBreakdown[booking.ticketType].second += booking.totalPrice;
        }
    }

    report << "Breakdown by Ticket Type:\n";
    for (const auto &entry : ticketBreakdown)
    {
        report << "  " << entry.first << ": " << entry.second.first
               << " tickets | Revenue: $" << entry.second.second << "\n";
    }
    if (ticketBreakdown.empty())
    {
        report << "  No active bookings yet.\n";
    }
    report << "\n========================================\n";

    return report.str();
}

std::string AdminService::generateCustomerReport() const
{
    std::ostringstream report;

    report << "\n========================================\n";
    report << "       CUSTOMER ACTIVITY REPORT\n";
    report << "========================================\n\n";

    // Get data from database
    int totalCustomers = Database::getInstance().getTotalCustomers();
    auto customers = Database::getInstance().getAllCustomers();

    report << "Total Registered Customers: " << totalCustomers << "\n\n";

    // Get booking counts per customer
    std::vector<std::pair<std::string, int>> customerBookings;
    for (const auto &customer : customers)
    {
        auto bookings = Database::getInstance().getBookingsByUserId(customer.id);
        customerBookings.push_back({customer.fullName, static_cast<int>(bookings.size())});
    }

    // Sort by booking count (descending)
    std::sort(customerBookings.begin(), customerBookings.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
              { return a.second > b.second; });

    report << "Top Customers by Bookings:\n";
    int rank = 1;
    for (const auto &entry : customerBookings)
    {
        if (rank > 5)
            break; // Show top 5
        report << "  " << rank << ". " << entry.first << " - " << entry.second << " bookings\n";
        rank++;
    }
    if (customerBookings.empty())
    {
        report << "  No customers with bookings yet.\n";
    }
    report << "\n========================================\n";

    return report.str();
}

std::string AdminService::generateBookingStatistics() const
{
    std::ostringstream report;

    report << "\n========================================\n";
    report << "      BOOKING STATISTICS\n";
    report << "========================================\n\n";

    // Get data from database
    int totalBookings = Database::getInstance().getTotalBookings();
    int cancelledBookings = Database::getInstance().getCancelledBookings();
    double totalRevenue = Database::getInstance().getTotalRevenue();
    int activeBookings = Database::getInstance().getActiveBookings();

    // Calculate statistics
    double avgBookingValue = (activeBookings > 0) ? (totalRevenue / activeBookings) : 0.0;
    double cancellationRate = (totalBookings > 0) ? (static_cast<double>(cancelledBookings) / totalBookings * 100) : 0.0;

    // Calculate average tickets per booking
    auto allBookings = Database::getInstance().getAllBookings();
    int totalTickets = 0;
    for (const auto &booking : allBookings)
    {
        if (booking.status == "active")
        {
            totalTickets += booking.numTickets;
        }
    }
    double avgTicketsPerBooking = (activeBookings > 0) ? (static_cast<double>(totalTickets) / activeBookings) : 0.0;

    report << std::fixed << std::setprecision(2);
    report << "Average Booking Value: $" << avgBookingValue << "\n";
    report << "Average Tickets per Booking: " << avgTicketsPerBooking << "\n";
    report << "Cancellation Rate: " << cancellationRate << "%\n\n";

    report << "Booking Summary:\n";
    report << "  Total Bookings: " << totalBookings << "\n";
    report << "  Active Bookings: " << activeBookings << "\n";
    report << "  Cancelled Bookings: " << cancelledBookings << "\n\n";

    report << "========================================\n";

    return report.str();
}

bool AdminService::validateAdminAccess(std::shared_ptr<Admin> admin,
                                       const std::string &operation) const
{
    // All admins have full access since we removed admin levels
    (void)operation; // Suppress unused parameter warning
    return admin != nullptr;
}

void AdminService::displayCustomerList() const
{
    auto customers = viewAllCustomers();

    std::cout << "\n--- Customer List ---" << std::endl;
    if (customers.empty())
    {
        std::cout << "No customers found." << std::endl;
        return;
    }

    std::cout << "Registered Customers:" << std::endl;
    for (const auto &customer : customers)
    {
        std::cout << customer << std::endl;
    }
}

void AdminService::displayReportsMenu() const
{
    std::cout << "\n--- Generate Reports ---" << std::endl;
    std::cout << "1. Sales Report" << std::endl;
    std::cout << "2. Customer Activity Report" << std::endl;
    std::cout << "3. Booking Statistics" << std::endl;
    std::cout << "4. Back to Admin Menu" << std::endl;
}

std::string AdminService::formatBookingData() const
{
    // TODO: Format booking data for reports
    return "Formatted booking data";
}

double AdminService::calculateTotalRevenue() const
{
    // Get total revenue from database
    return Database::getInstance().getTotalRevenue();
}

std::vector<std::string> AdminService::viewAllVehicles() const
{
    std::vector<std::string> vehicles;

    // Retrieve from database
    auto vehicleRecords = Database::getInstance().getAllVehicles();

    for (const auto &record : vehicleRecords)
    {
        std::ostringstream vehicleStr;
        vehicleStr << "ID: " << record.id << " | Type: " << record.vehicleType
                   << " | Capacity: " << record.capacity;

        if (record.vehicleType == "Plane")
        {
            vehicleStr << " | Airline: " << record.airline
                       << " | Flight: " << record.flightNumber;
        }
        else if (record.vehicleType == "Cab")
        {
            vehicleStr << " | License: " << record.licensePlate
                       << " | Driver: " << record.driverName;
        }
        else if (record.vehicleType == "Train")
        {
            vehicleStr << " | Train#: " << record.trainNumber
                       << " | Platform: " << record.platform;
        }

        vehicles.push_back(vehicleStr.str());
    }

    return vehicles;
}

void AdminService::displayVehicleList() const
{
    auto vehicles = viewAllVehicles();

    std::cout << "\n--- Vehicle List ---" << std::endl;
    if (vehicles.empty())
    {
        std::cout << "No vehicles found." << std::endl;
        return;
    }

    std::cout << "Available Vehicles:" << std::endl;
    for (const auto &vehicle : vehicles)
    {
        std::cout << vehicle << std::endl;
    }
}
