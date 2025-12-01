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
     * @return true if successful initialization
     */
    bool initialize(const std::string &dbPath = "database.db");

    /**
     * Close the database connection
     */
    void close();

    // USER OPERATIONS
    /**
     * Creates a new user in the database
     * @param username, password, fullName, email, phone, userType requried user fields
     * @return true if user creation successful
     */
    bool createUser(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &email,
                    const std::string &phone, const std::string &userType);

    /**
     * retrieves a user by username
     * @param username required user's field to search
     * @return UserRecord structure with the user's details
     */
    UserRecord getUserByUsername(const std::string &username);

    /**
     * retrieves a user by ID
     * @param userId The database ID of the user
     * @return UserRecord structure with the user's details
     */
    UserRecord getUserById(int userId);

    /**
     * retrieves all customers
     * @return Vector of UserRecord structures for all customers
     */
    std::vector<UserRecord> getAllCustomers();

    /**
     * Check if a user exists by username
     * @param username The username to check
     * @return true if user exists
     */
    bool userExists(const std::string &username);

    // TICKET OPERATIONS
    /**
     * retrieves all tickets
     * @return Vector of TicketInfo structures for all tickets
     */
    std::vector<TicketInfo> getAllTickets();

    /**
     * retrieves a ticket by ID
     * @param ticketId The database ID of the ticket
     * @return TicketInfo structure with the ticket's details
     */
    TicketInfo getTicketById(int ticketId);

    /**
     * retrieves a ticket by type
     * @param type The type of the ticket
     * @return TicketInfo structure with the ticket's details
     */
    TicketInfo getTicketByType(const std::string &type);

    /**
     * retrieves a ticket ID by type
     * @param type The type of the ticket
     * @return ticketId The database ID of the ticket
     */
    int getTicketIdByType(const std::string &type);

    /**
     * creates a new ticket in the database
     * @param ticket The TicketInfo structure containing ticket details
     * @return true if ticket creation successful
     */
    bool createTicket(const TicketInfo &ticket);

    /**
     * updates ticket availability
     * @param ticketId The database ID of the ticket
     * @param change The change in availability (negative for bookings, positive for cancellations)
     * @return true if update successful
     */
    bool updateTicketAvailability(int ticketId, int change);

    /**
     * updates a ticket by ID
     * @param ticketId The database ID of the ticket
     * @param ticket The TicketInfo structure containing new ticket details
     * @return true if update successful
     */
    bool updateTicketById(int ticketId, const TicketInfo &ticket);

    /**
     * deletes a ticket by ID
     * @param ticketId The database ID of the ticket
     * @return true if deletion successful
     */
    bool deleteTicketById(int ticketId);

    /**
     * deletes a ticket by type
     * @param type The type of the ticket
     * @return true if deletion successful
     */
    bool deleteTicket(const std::string &type);

    /**
     * Check if a ticket exists by type
     * @param type The type of the ticket
     * @return true if ticket exists
     */
    bool ticketExists(const std::string &type);

    // VEHICLE OPERATIONS
    /**
     * creates a new vehicle in the database
     * @param vehicle The VehicleRecord structure containing vehicle details
     * @return true if vehicle creation successful
     */
    bool createVehicle(const VehicleRecord &vehicle);

    /**
     * retrieves all vehicles
     * @return Vector of VehicleRecord structures for all vehicles
     */
    std::vector<VehicleRecord> getAllVehicles();

    /**
     * retrieves a vehicle by ID
     * @param vehicleId The database ID of the vehicle
     * @return VehicleRecord structure with the vehicle's details
     */
    VehicleRecord getVehicleById(int vehicleId);

    /**
     * retrieves vehicles by type
     * @param type The type of the vehicle ("Plane", "Cab", or "Train")
     * @return Vector of VehicleRecord structures for vehicles of the specified type
     */
    std::vector<VehicleRecord> getVehiclesByType(const std::string &type);

    /**
     * updates a vehicle by ID
     * @param vehicleId The database ID of the vehicle
     * @param vehicle The VehicleRecord structure containing new vehicle details
     * @return true if update successful
     */
    bool updateVehicle(int vehicleId, const VehicleRecord &vehicle);

    /**
     * deletes a vehicle by ID
     * @param vehicleId The database ID of the vehicle
     * @return true if deletion successful
     */
    bool deleteVehicle(int vehicleId);

    /**
     * creates the appropriate vehicle object from a database record
     * @param record The VehicleRecord structure containing vehicle details
     * @return Pointer to the created vehicle object
     */
    std::shared_ptr<BaseVehicle> createVehicleObject(const VehicleRecord &record);

    // BOOKING OPERATIONS
    /**
     * creates a new booking in the database
     * @param userId, ticketId, ticketType, numTickets, totalPrice required booking fields
     * @return Booking ID string if booking creation successful
     */
    std::string createBooking(int userId, int ticketId, const std::string &ticketType,
                              int numTickets, double totalPrice);

    /**
     * cancels a booking by ID
     * @param bookingId The ID of the booking to cancel
     * @return true if cancellation successful
     */
    bool cancelBooking(const std::string &bookingId);

    /**
     * retrieves bookings by user ID
     * @param userId The database ID of the user
     * @return Vector of BookingRecord structures for the user's bookings
     */
    std::vector<BookingRecord> getBookingsByUserId(int userId);

    /**
     * retrieves all bookings
     * @return Vector of BookingRecord structures for all bookings
     */
    std::vector<BookingRecord> getAllBookings();

    /**
     * retrieves a booking by ID
     * @param bookingId The booking ID string
     * @return BookingRecord structure with the booking's details
     */
    BookingRecord getBookingById(const std::string &bookingId);

    /**
     * Check if a booking exists by booking ID
     * @param bookingId The booking ID string
     * @return true if booking exists
     */
    bool bookingExists(const std::string &bookingId);

    // REPORT OPERATIONS
    /**
     * Get total number of bookings
     * @return Total bookings count
     */
    int getTotalBookings();

    /**
     * Get number of active bookings
     * @return Active bookings count
     */
    int getActiveBookings();

    /**
     * Get number of cancelled bookings
     * @return Cancelled bookings count
     */
    int getCancelledBookings();

    /**
     * Get total revenue from bookings
     * @return Total revenue
     */
    double getTotalRevenue();

    /**
     * Get total number of customers
     * @return Total customers count
     */
    int getTotalCustomers();

    // PREVENT COPYING
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