#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>
#include "TicketService.h"

// Forward declarations
class Customer;
class Admin;
class BaseUser;
class BaseVehicle;
class Plane;
class Cab;
class Train;

/**
 * Struct to hold vehicle information from database
 */
struct VehicleRecord
{
    int id;
    std::string vehicleType; // "Plane", "Cab", "Train"
    int capacity;

    // Plane-specific fields
    std::string airline;
    std::string flightNumber;
    std::string flightNo;

    // Cab-specific fields
    std::string licensePlate;
    std::string driverName;

    // Train-specific fields
    std::string trainNumber;
    std::string platform;
};

/**
 * Struct to hold booking information from database
 */
struct BookingRecord
{
    int id;
    std::string bookingId;
    int userId;
    int ticketId;
    std::string ticketType;
    int numTickets;
    double totalPrice;
    std::string status;
    std::string createdAt;
    std::string userName; // For admin view
};

/**
 * Struct to hold user information from database
 */
struct UserRecord
{
    int id;
    std::string username;
    std::string password;
    std::string fullName;
    std::string email;
    std::string phone;
    std::string userType; // "admin" or "customer"
};

/**
 * Database class - Singleton pattern for SQLite database operations
 */
class Database
{
public:
    /**
     * Get the singleton instance
     */
    static Database &getInstance();

    /**
     * Initialize the database connection and create tables
     * @param dbPath Path to the database file
     * @return true if initialization successful
     */
    bool initialize(const std::string &dbPath = "database.db");

    /**
     * Close the database connection
     */
    void close();

    // User operations
    bool createUser(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &email,
                    const std::string &phone, const std::string &userType);

    UserRecord getUserByUsername(const std::string &username);
    UserRecord getUserById(int userId);
    std::vector<UserRecord> getAllCustomers();
    bool userExists(const std::string &username);

    // Ticket operations
    std::vector<TicketInfo> getAllTickets();
    TicketInfo getTicketById(int ticketId);
    TicketInfo getTicketByType(const std::string &type);
    int getTicketIdByType(const std::string &type);
    bool createTicket(const TicketInfo &ticket);
    bool updateTicketAvailability(int ticketId, int change);
    bool updateTicketById(int ticketId, const TicketInfo &ticket);
    bool deleteTicketById(int ticketId);
    bool deleteTicket(const std::string &type);
    bool ticketExists(const std::string &type);

    // Vehicle operations
    bool createVehicle(const VehicleRecord &vehicle);
    std::vector<VehicleRecord> getAllVehicles();
    VehicleRecord getVehicleById(int vehicleId);
    std::vector<VehicleRecord> getVehiclesByType(const std::string &type);
    bool updateVehicle(int vehicleId, const VehicleRecord &vehicle);
    bool deleteVehicle(int vehicleId);
    std::shared_ptr<BaseVehicle> createVehicleObject(const VehicleRecord &record);

    // Booking operations
    std::string createBooking(int userId, int ticketId, const std::string &ticketType,
                              int numTickets, double totalPrice);
    bool cancelBooking(const std::string &bookingId);
    std::vector<BookingRecord> getBookingsByUserId(int userId);
    std::vector<BookingRecord> getAllBookings();
    BookingRecord getBookingById(const std::string &bookingId);
    bool bookingExists(const std::string &bookingId);

    // Report operations
    int getTotalBookings();
    int getActiveBookings();
    int getCancelledBookings();
    double getTotalRevenue();
    int getTotalCustomers();

    // Prevent copying
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

private:
    Database() : db(nullptr) {}
    ~Database();

    bool createTables();
    bool seedInitialData();
    bool executeQuery(const std::string &query);

    sqlite3 *db;
    static Database *instance;
};

#endif