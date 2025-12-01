#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
class Admin;

/**
 * AdminService handles all business logic related to administrative operations.
 * eg:
 */
class AdminService
{
public:
    AdminService() = default;

    /**
     * View all customers in the system
     * @return Vector of customer information strings
     */
    std::vector<std::string> viewAllCustomers() const;

    /**
     * Generate a sales report
     * @return Sales report as a formatted string
     */
    std::string generateSalesReport() const;

    /**
     * Generate a customer activity report
     * @return Customer report as a formatted string
     */
    std::string generateCustomerReport() const;

    /**
     * Generate a booking statistics report
     * @return Booking statistics as a formatted string
     */
    std::string generateBookingStatistics() const;

    /**
     * Checks if an admin or not
     * @param admin The admin user
     * @param operation The operation to validate
     * @return true if admin has permission, false otherwise
     */
    bool validateAdminAccess(std::shared_ptr<Admin> admin,
                             const std::string &operation) const;

    /**
     * Display customer list in formatted output
     */
    void displayCustomerList() const;

    /**
     * Display all reports menu and options
     */
    void displayReportsMenu() const;

    /**
     * View all vehicles in the system
     * @return Vector of vehicle information strings
     */
    std::vector<std::string> viewAllVehicles() const;

    /**
     * Display all vehicles in formatted output
     */
    void displayVehicleList() const;

private:
    /**
     * Format booking data for reporting
     * @return Formatted booking data
     */
    std::string formatBookingData() const;

    /**
     * Calculate total revenue from bookings
     * @return Total revenue
     */
    double calculateTotalRevenue() const;
};

#endif
