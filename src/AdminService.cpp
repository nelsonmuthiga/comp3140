#include "AdminService.h"
#include "Admin.h"
#include <iostream>
#include <sstream>

std::vector<std::string> AdminService::viewAllCustomers() const
{
    std::vector<std::string> customers;

    // TODO: Retrieve from database
    // Placeholder data
    customers.push_back("ID: 1001 | Name: John Doe | Email: john@example.com | Phone: 555-0101");
    customers.push_back("ID: 1002 | Name: Jane Smith | Email: jane@example.com | Phone: 555-0102");
    customers.push_back("ID: 1003 | Name: Mike Jones | Email: mike@example.com | Phone: 555-0103");

    return customers;
}

std::string AdminService::generateSalesReport() const
{
    std::ostringstream report;

    report << "\n========================================\n";
    report << "         SALES REPORT\n";
    report << "========================================\n\n";

    // TODO: Calculate from actual database
    double totalRevenue = calculateTotalRevenue();
    int totalBookings = 45;
    int activeBookings = 38;
    int cancelledBookings = 7;

    report << "Total Revenue:       $" << totalRevenue << "\n";
    report << "Total Bookings:      " << totalBookings << "\n";
    report << "Active Bookings:     " << activeBookings << "\n";
    report << "Cancelled Bookings:  " << cancelledBookings << "\n\n";

    report << "Breakdown by Ticket Type:\n";
    report << "  Cab:    15 bookings | Revenue: $375\n";
    report << "  Plane:  10 bookings | Revenue: $4,500\n";
    report << "  Train:  20 bookings | Revenue: $1,700\n\n";

    report << "========================================\n";

    return report.str();
}

std::string AdminService::generateCustomerReport() const
{
    std::ostringstream report;

    report << "\n========================================\n";
    report << "       CUSTOMER ACTIVITY REPORT\n";
    report << "========================================\n\n";

    // TODO: Calculate from actual database
    report << "Total Registered Customers: 127\n";
    report << "Active Customers (last 30 days): 89\n";
    report << "New Customers (this month): 12\n\n";

    report << "Top Customers by Bookings:\n";
    report << "  1. John Doe - 8 bookings\n";
    report << "  2. Jane Smith - 6 bookings\n";
    report << "  3. Mike Jones - 5 bookings\n\n";

    report << "========================================\n";

    return report.str();
}

std::string AdminService::generateBookingStatistics() const
{
    std::ostringstream report;

    report << "\n========================================\n";
    report << "      BOOKING STATISTICS\n";
    report << "========================================\n\n";

    // TODO: Calculate from actual database
    report << "Average Booking Value: $145.50\n";
    report << "Average Tickets per Booking: 2.3\n";
    report << "Peak Booking Hours: 10 AM - 2 PM\n";
    report << "Cancellation Rate: 15.6%\n\n";

    report << "Monthly Trend:\n";
    report << "  October:  38 bookings\n";
    report << "  November: 45 bookings (current)\n\n";

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
    // TODO: Calculate from actual bookings in database
    // Placeholder calculation
    return 6575.00;
}
