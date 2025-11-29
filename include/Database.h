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
    TicketInfo getTicketByType(const std::string &type);
    int getTicketIdByType(const std::string &type);
    bool createTicket(const TicketInfo &ticket);
    bool updateTicketAvailability(const std::string &type, int change);
    bool deleteTicket(const std::string &type);
    bool ticketExists(const std::string &type);

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