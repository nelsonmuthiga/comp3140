#include "Database.h"
#include "Plane.h"
#include "Cab.h"
#include "Train.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>

Database *Database::instance = nullptr;

Database &Database::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Database();
    }
    return *instance;
}

Database::~Database()
{
    close();
}

bool Database::initialize(const std::string &dbPath)
{
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!createTables())
    {
        std::cerr << "Failed to create tables" << std::endl;
        return false;
    }

    // Seed initial data if tables are empty
    seedInitialData();

    return true;
}

void Database::close()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::createTables()
{
    const char *userTableSQL = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            full_name TEXT NOT NULL,
            email TEXT UNIQUE NOT NULL,
            phone TEXT,
            user_type TEXT NOT NULL CHECK(user_type IN ('admin', 'customer')),
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    const char *ticketsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS tickets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL,
            origin TEXT NOT NULL,
            destination TEXT NOT NULL,
            price REAL NOT NULL CHECK(price > 0),
            availability INTEGER NOT NULL DEFAULT 0 CHECK(availability >= 0),
            date TEXT,
            vehicle_id INTEGER,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (vehicle_id) REFERENCES vehicles(id)
        );
    )";

    const char *bookingsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS bookings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            booking_id TEXT UNIQUE NOT NULL,
            user_id INTEGER NOT NULL,
            ticket_id INTEGER NOT NULL,
            ticket_type TEXT NOT NULL,
            num_tickets INTEGER NOT NULL CHECK(num_tickets > 0),
            total_price REAL NOT NULL,
            status TEXT NOT NULL DEFAULT 'active' CHECK(status IN ('active', 'cancelled')),
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (ticket_id) REFERENCES tickets(id)
        );
    )";

    const char *vehiclesTableSQL = R"(
        CREATE TABLE IF NOT EXISTS vehicles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            vehicle_type TEXT NOT NULL CHECK(vehicle_type IN ('Plane', 'Cab', 'Train')),
            capacity INTEGER NOT NULL CHECK(capacity > 0),
            airline TEXT,
            flight_number TEXT,
            flight_no TEXT,
            license_plate TEXT,
            driver_name TEXT,
            train_number TEXT,
            platform TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    if (!executeQuery(userTableSQL))
        return false;
    if (!executeQuery(ticketsTableSQL))
        return false;
    if (!executeQuery(bookingsTableSQL))
        return false;
    if (!executeQuery(vehiclesTableSQL))
        return false;

    return true;
}

bool Database::seedInitialData()
{
    // Check if admin exists
    if (!userExists("admin"))
    {
        createUser("admin", "admin", "Administrator", "admin@ticketsystem.com", "", "admin");
    }

    // Check if vehicles exist - seed sample vehicles using OOP!
    std::vector<VehicleRecord> vehicles = getAllVehicles();
    if (vehicles.empty())
    {
        // Create sample Planes using Plane class
        Plane plane1(0, "AirExpress", "AE123", "AE123", 180);
        VehicleRecord plane1Record;
        plane1Record.vehicleType = plane1.getVehicleType();
        plane1Record.capacity = plane1.getVehicleCapacity();
        plane1Record.airline = plane1.getAirline();
        plane1Record.flightNumber = plane1.getFlightNumber();
        plane1Record.flightNo = plane1.getFlightNo();
        createVehicle(plane1Record);

        Plane plane2(0, "SkyWings", "SW456", "SW456", 220);
        VehicleRecord plane2Record;
        plane2Record.vehicleType = plane2.getVehicleType();
        plane2Record.capacity = plane2.getVehicleCapacity();
        plane2Record.airline = plane2.getAirline();
        plane2Record.flightNumber = plane2.getFlightNumber();
        plane2Record.flightNo = plane2.getFlightNo();
        createVehicle(plane2Record);

        // Create sample Cabs using Cab class
        Cab cab1(0, "ABC123", "John Smith", 4);
        VehicleRecord cab1Record;
        cab1Record.vehicleType = cab1.getVehicleType();
        cab1Record.capacity = cab1.getVehicleCapacity();
        cab1Record.licensePlate = cab1.getLicensePlate();
        cab1Record.driverName = cab1.getDriverName();
        createVehicle(cab1Record);

        Cab cab2(0, "XYZ789", "Mary Johnson", 6);
        VehicleRecord cab2Record;
        cab2Record.vehicleType = cab2.getVehicleType();
        cab2Record.capacity = cab2.getVehicleCapacity();
        cab2Record.licensePlate = cab2.getLicensePlate();
        cab2Record.driverName = cab2.getDriverName();
        createVehicle(cab2Record);

        // Create sample Trains using Train class
        Train train1(0, "TR100", "Platform 1", 500);
        VehicleRecord train1Record;
        train1Record.vehicleType = train1.getVehicleType();
        train1Record.capacity = train1.getVehicleCapacity();
        train1Record.trainNumber = train1.getTrainNumber();
        train1Record.platform = train1.getPlatform();
        createVehicle(train1Record);

        Train train2(0, "TR200", "Platform 2", 450);
        VehicleRecord train2Record;
        train2Record.vehicleType = train2.getVehicleType();
        train2Record.capacity = train2.getVehicleCapacity();
        train2Record.trainNumber = train2.getTrainNumber();
        train2Record.platform = train2.getPlatform();
        createVehicle(train2Record);
    }

    // Check if tickets exist
    std::vector<TicketInfo> tickets = getAllTickets();
    if (tickets.empty())
    {
        // Sample CAB tickets
        createTicket({0, "CAB", "New York", "Brooklyn", 25.0, 50, "2025-12-01", 0});
        createTicket({0, "CAB", "Manhattan", "JFK Airport", 45.0, 30, "2025-12-02", 0});
        createTicket({0, "CAB", "Downtown", "Central Station", 15.0, 40, "2025-12-03", 0});

        // Sample PLANE tickets
        createTicket({0, "PLANE", "Los Angeles", "New York", 450.0, 20, "2025-12-05", 0});
        createTicket({0, "PLANE", "Chicago", "Miami", 320.0, 35, "2025-12-07", 0});
        createTicket({0, "PLANE", "San Francisco", "Seattle", 180.0, 50, "2025-12-08", 0});
        createTicket({0, "PLANE", "Boston", "Los Angeles", 520.0, 25, "2025-12-10", 0});

        // Sample TRAIN tickets
        createTicket({0, "TRAIN", "Boston", "Washington DC", 85.0, 100, "2025-12-10", 0});
        createTicket({0, "TRAIN", "New York", "Philadelphia", 55.0, 120, "2025-12-11", 0});
        createTicket({0, "TRAIN", "Chicago", "Detroit", 65.0, 80, "2025-12-12", 0});
        createTicket({0, "TRAIN", "Portland", "Sacramento", 95.0, 90, "2025-12-15", 0});

        // Sample BUS tickets
        createTicket({0, "BUS", "Austin", "Houston", 35.0, 60, "2025-12-14", 0});
        createTicket({0, "BUS", "Phoenix", "Tucson", 28.0, 45, "2025-12-16", 0});
    }

    return true;
}

bool Database::executeQuery(const std::string &query)
{
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// User operations
bool Database::createUser(const std::string &username, const std::string &password,
                          const std::string &fullName, const std::string &email,
                          const std::string &phone, const std::string &userType)
{
    const char *sql = "INSERT INTO users (username, password, full_name, email, phone, user_type) VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, fullName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, phone.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, userType.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

UserRecord Database::getUserByUsername(const std::string &username)
{
    UserRecord user = {0, "", "", "", "", "", ""};
    const char *sql = "SELECT id, username, password, full_name, email, phone, user_type FROM users WHERE username = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return user;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user.id = sqlite3_column_int(stmt, 0);
        user.username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        user.password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        user.fullName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        user.email = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        user.phone = phone ? phone : "";
        user.userType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
    }

    sqlite3_finalize(stmt);
    return user;
}

UserRecord Database::getUserById(int userId)
{
    UserRecord user = {0, "", "", "", "", "", ""};
    const char *sql = "SELECT id, username, password, full_name, email, phone, user_type FROM users WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return user;
    }

    sqlite3_bind_int(stmt, 1, userId);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user.id = sqlite3_column_int(stmt, 0);
        user.username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        user.password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        user.fullName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        user.email = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        user.phone = phone ? phone : "";
        user.userType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
    }

    sqlite3_finalize(stmt);
    return user;
}

std::vector<UserRecord> Database::getAllCustomers()
{
    std::vector<UserRecord> customers;
    const char *sql = "SELECT id, username, password, full_name, email, phone, user_type FROM users WHERE user_type = 'customer';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return customers;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        UserRecord user;
        user.id = sqlite3_column_int(stmt, 0);
        user.username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        user.password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        user.fullName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        user.email = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *phone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        user.phone = phone ? phone : "";
        user.userType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
        customers.push_back(user);
    }

    sqlite3_finalize(stmt);
    return customers;
}

bool Database::userExists(const std::string &username)
{
    const char *sql = "SELECT COUNT(*) FROM users WHERE username = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}

// Ticket operations
std::vector<TicketInfo> Database::getAllTickets()
{
    std::vector<TicketInfo> tickets;
    const char *sql = "SELECT id, type, origin, destination, price, availability, date, vehicle_id FROM tickets;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return tickets;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        TicketInfo ticket;
        ticket.id = sqlite3_column_int(stmt, 0);
        ticket.type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        ticket.origin = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        ticket.destination = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        ticket.price = sqlite3_column_double(stmt, 4);
        ticket.availability = sqlite3_column_int(stmt, 5);
        const char *date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
        ticket.date = date ? date : "";
        ticket.vehicleId = sqlite3_column_int(stmt, 7);
        tickets.push_back(ticket);
    }

    sqlite3_finalize(stmt);
    return tickets;
}

TicketInfo Database::getTicketById(int ticketId)
{
    TicketInfo ticket = {0, "", "", "", 0.0, 0, "", 0};
    const char *sql = "SELECT id, type, origin, destination, price, availability, date, vehicle_id FROM tickets WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return ticket;
    }

    sqlite3_bind_int(stmt, 1, ticketId);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        ticket.id = sqlite3_column_int(stmt, 0);
        ticket.type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        ticket.origin = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        ticket.destination = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        ticket.price = sqlite3_column_double(stmt, 4);
        ticket.availability = sqlite3_column_int(stmt, 5);
        const char *date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
        ticket.date = date ? date : "";
        ticket.vehicleId = sqlite3_column_int(stmt, 7);
    }

    sqlite3_finalize(stmt);
    return ticket;
}

TicketInfo Database::getTicketByType(const std::string &type)
{
    TicketInfo ticket = {0, "", "", "", 0.0, 0, "", 0};
    const char *sql = "SELECT id, type, origin, destination, price, availability, date, vehicle_id FROM tickets WHERE type = ? LIMIT 1;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return ticket;
    }

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        ticket.id = sqlite3_column_int(stmt, 0);
        ticket.type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        ticket.origin = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        ticket.destination = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        ticket.price = sqlite3_column_double(stmt, 4);
        ticket.availability = sqlite3_column_int(stmt, 5);
        const char *date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        ticket.date = date ? date : "";
        ticket.vehicleId = sqlite3_column_int(stmt, 6);
    }

    sqlite3_finalize(stmt);
    return ticket;
}

int Database::getTicketIdByType(const std::string &type)
{
    const char *sql = "SELECT id FROM tickets WHERE type = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);

    int id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return id;
}

bool Database::createTicket(const TicketInfo &ticket)
{
    const char *sql = "INSERT INTO tickets (type, origin, destination, price, availability, date, vehicle_id) VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, ticket.type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ticket.origin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ticket.destination.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, ticket.price);
    sqlite3_bind_int(stmt, 5, ticket.availability);
    sqlite3_bind_text(stmt, 6, ticket.date.c_str(), -1, SQLITE_TRANSIENT);

    if (ticket.vehicleId > 0)
    {
        sqlite3_bind_int(stmt, 7, ticket.vehicleId);
    }
    else
    {
        sqlite3_bind_null(stmt, 7);
    }

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

bool Database::updateTicketAvailability(int ticketId, int change)
{
    const char *sql = "UPDATE tickets SET availability = availability + ? WHERE id = ? AND availability + ? >= 0;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, change);
    sqlite3_bind_int(stmt, 2, ticketId);
    sqlite3_bind_int(stmt, 3, change);

    bool success = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return success;
}

bool Database::updateTicketById(int ticketId, const TicketInfo &ticket)
{
    const char *sql = "UPDATE tickets SET type = ?, origin = ?, destination = ?, price = ?, availability = ?, date = ?, vehicle_id = ? WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, ticket.type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ticket.origin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, ticket.destination.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, ticket.price);
    sqlite3_bind_int(stmt, 5, ticket.availability);
    sqlite3_bind_text(stmt, 6, ticket.date.c_str(), -1, SQLITE_TRANSIENT);

    if (ticket.vehicleId > 0)
    {
        sqlite3_bind_int(stmt, 7, ticket.vehicleId);
    }
    else
    {
        sqlite3_bind_null(stmt, 7);
    }

    sqlite3_bind_int(stmt, 8, ticketId);

    bool success = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return success;
}

bool Database::deleteTicketById(int ticketId)
{
    const char *sql = "DELETE FROM tickets WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, ticketId);

    bool success = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return success;
}

bool Database::deleteTicket(const std::string &type)
{
    const char *sql = "DELETE FROM tickets WHERE type = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return success;
}

bool Database::ticketExists(const std::string &type)
{
    const char *sql = "SELECT COUNT(*) FROM tickets WHERE type = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}

// Booking operations
std::string Database::createBooking(int userId, int ticketId, const std::string &ticketType,
                                    int numTickets, double totalPrice)
{
    // Generate booking ID
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
    int randomNum = std::rand() % 10000 + 1000;
    std::string bookingId = "BK" + std::to_string(randomNum);

    const char *sql = "INSERT INTO bookings (booking_id, user_id, ticket_id, ticket_type, num_tickets, total_price, status) VALUES (?, ?, ?, ?, ?, ?, 'active');";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return "";
    }

    sqlite3_bind_text(stmt, 1, bookingId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_bind_int(stmt, 3, ticketId);
    sqlite3_bind_text(stmt, 4, ticketType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, numTickets);
    sqlite3_bind_double(stmt, 6, totalPrice);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success ? bookingId : "";
}

bool Database::cancelBooking(const std::string &bookingId)
{
    const char *sql = "UPDATE bookings SET status = 'cancelled' WHERE booking_id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, bookingId.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return success;
}

std::vector<BookingRecord> Database::getBookingsByUserId(int userId)
{
    std::vector<BookingRecord> bookings;
    const char *sql = "SELECT id, booking_id, user_id, ticket_id, ticket_type, num_tickets, total_price, status, created_at FROM bookings WHERE user_id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return bookings;
    }

    sqlite3_bind_int(stmt, 1, userId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        BookingRecord booking;
        booking.id = sqlite3_column_int(stmt, 0);
        booking.bookingId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        booking.userId = sqlite3_column_int(stmt, 2);
        booking.ticketId = sqlite3_column_int(stmt, 3);
        booking.ticketType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        booking.numTickets = sqlite3_column_int(stmt, 5);
        booking.totalPrice = sqlite3_column_double(stmt, 6);
        booking.status = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        const char *createdAt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));
        booking.createdAt = createdAt ? createdAt : "";
        bookings.push_back(booking);
    }

    sqlite3_finalize(stmt);
    return bookings;
}

std::vector<BookingRecord> Database::getAllBookings()
{
    std::vector<BookingRecord> bookings;
    const char *sql = R"(
        SELECT b.id, b.booking_id, b.user_id, b.ticket_id, b.ticket_type, b.num_tickets, 
               b.total_price, b.status, b.created_at, u.username 
        FROM bookings b 
        JOIN users u ON b.user_id = u.id;
    )";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return bookings;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        BookingRecord booking;
        booking.id = sqlite3_column_int(stmt, 0);
        booking.bookingId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        booking.userId = sqlite3_column_int(stmt, 2);
        booking.ticketId = sqlite3_column_int(stmt, 3);
        booking.ticketType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        booking.numTickets = sqlite3_column_int(stmt, 5);
        booking.totalPrice = sqlite3_column_double(stmt, 6);
        booking.status = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        const char *createdAt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));
        booking.createdAt = createdAt ? createdAt : "";
        booking.userName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9));
        bookings.push_back(booking);
    }

    sqlite3_finalize(stmt);
    return bookings;
}

BookingRecord Database::getBookingById(const std::string &bookingId)
{
    BookingRecord booking = {0, "", 0, 0, "", 0, 0.0, "", "", ""};
    const char *sql = "SELECT id, booking_id, user_id, ticket_id, ticket_type, num_tickets, total_price, status, created_at FROM bookings WHERE booking_id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return booking;
    }

    sqlite3_bind_text(stmt, 1, bookingId.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        booking.id = sqlite3_column_int(stmt, 0);
        booking.bookingId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        booking.userId = sqlite3_column_int(stmt, 2);
        booking.ticketId = sqlite3_column_int(stmt, 3);
        booking.ticketType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        booking.numTickets = sqlite3_column_int(stmt, 5);
        booking.totalPrice = sqlite3_column_double(stmt, 6);
        booking.status = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        const char *createdAt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));
        booking.createdAt = createdAt ? createdAt : "";
    }

    sqlite3_finalize(stmt);
    return booking;
}

bool Database::bookingExists(const std::string &bookingId)
{
    const char *sql = "SELECT COUNT(*) FROM bookings WHERE booking_id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, bookingId.c_str(), -1, SQLITE_TRANSIENT);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}

// Report operations
int Database::getTotalBookings()
{
    const char *sql = "SELECT COUNT(*) FROM bookings;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

int Database::getActiveBookings()
{
    const char *sql = "SELECT COUNT(*) FROM bookings WHERE status = 'active';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

int Database::getCancelledBookings()
{
    const char *sql = "SELECT COUNT(*) FROM bookings WHERE status = 'cancelled';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

double Database::getTotalRevenue()
{
    const char *sql = "SELECT COALESCE(SUM(total_price), 0) FROM bookings WHERE status = 'active';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0.0;
    }

    double revenue = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        revenue = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return revenue;
}

int Database::getTotalCustomers()
{
    const char *sql = "SELECT COUNT(*) FROM users WHERE user_type = 'customer';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

// Vehicle operations
bool Database::createVehicle(const VehicleRecord &vehicle)
{
    const char *sql = R"(
        INSERT INTO vehicles (vehicle_type, capacity, airline, flight_number, flight_no, 
                            license_plate, driver_name, train_number, platform) 
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, vehicle.vehicleType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, vehicle.capacity);
    sqlite3_bind_text(stmt, 3, vehicle.airline.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, vehicle.flightNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, vehicle.flightNo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, vehicle.licensePlate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, vehicle.driverName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, vehicle.trainNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, vehicle.platform.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

std::vector<VehicleRecord> Database::getAllVehicles()
{
    std::vector<VehicleRecord> vehicles;
    const char *sql = "SELECT * FROM vehicles ORDER BY id;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return vehicles;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        VehicleRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.vehicleType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        record.capacity = sqlite3_column_int(stmt, 2);

        const char *airline = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        record.airline = airline ? airline : "";

        const char *flightNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        record.flightNumber = flightNumber ? flightNumber : "";

        const char *flightNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        record.flightNo = flightNo ? flightNo : "";

        const char *licensePlate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
        record.licensePlate = licensePlate ? licensePlate : "";

        const char *driverName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        record.driverName = driverName ? driverName : "";

        const char *trainNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));
        record.trainNumber = trainNumber ? trainNumber : "";

        const char *platform = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9));
        record.platform = platform ? platform : "";

        vehicles.push_back(record);
    }

    sqlite3_finalize(stmt);
    return vehicles;
}

VehicleRecord Database::getVehicleById(int vehicleId)
{
    VehicleRecord record;
    record.id = 0; // Indicate not found

    const char *sql = "SELECT * FROM vehicles WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return record;
    }

    sqlite3_bind_int(stmt, 1, vehicleId);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        record.id = sqlite3_column_int(stmt, 0);
        record.vehicleType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        record.capacity = sqlite3_column_int(stmt, 2);

        const char *airline = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        record.airline = airline ? airline : "";

        const char *flightNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        record.flightNumber = flightNumber ? flightNumber : "";

        const char *flightNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        record.flightNo = flightNo ? flightNo : "";

        const char *licensePlate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
        record.licensePlate = licensePlate ? licensePlate : "";

        const char *driverName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        record.driverName = driverName ? driverName : "";

        const char *trainNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));
        record.trainNumber = trainNumber ? trainNumber : "";

        const char *platform = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9));
        record.platform = platform ? platform : "";
    }

    sqlite3_finalize(stmt);
    return record;
}

std::vector<VehicleRecord> Database::getVehiclesByType(const std::string &type)
{
    std::vector<VehicleRecord> vehicles;
    const char *sql = "SELECT * FROM vehicles WHERE vehicle_type = ? ORDER BY id;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return vehicles;
    }

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        VehicleRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.vehicleType = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        record.capacity = sqlite3_column_int(stmt, 2);

        const char *airline = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        record.airline = airline ? airline : "";

        const char *flightNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        record.flightNumber = flightNumber ? flightNumber : "";

        const char *flightNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        record.flightNo = flightNo ? flightNo : "";

        const char *licensePlate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));
        record.licensePlate = licensePlate ? licensePlate : "";

        const char *driverName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7));
        record.driverName = driverName ? driverName : "";

        const char *trainNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 8));
        record.trainNumber = trainNumber ? trainNumber : "";

        const char *platform = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9));
        record.platform = platform ? platform : "";

        vehicles.push_back(record);
    }

    sqlite3_finalize(stmt);
    return vehicles;
}

bool Database::updateVehicle(int vehicleId, const VehicleRecord &vehicle)
{
    const char *sql = R"(
        UPDATE vehicles 
        SET vehicle_type = ?, capacity = ?, airline = ?, flight_number = ?, flight_no = ?,
            license_plate = ?, driver_name = ?, train_number = ?, platform = ?
        WHERE id = ?;
    )";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, vehicle.vehicleType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, vehicle.capacity);
    sqlite3_bind_text(stmt, 3, vehicle.airline.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, vehicle.flightNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, vehicle.flightNo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, vehicle.licensePlate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, vehicle.driverName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, vehicle.trainNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, vehicle.platform.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 10, vehicleId);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

bool Database::deleteVehicle(int vehicleId)
{
    const char *sql = "DELETE FROM vehicles WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, vehicleId);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

std::shared_ptr<BaseVehicle> Database::createVehicleObject(const VehicleRecord &record)
{
    // This function creates the appropriate vehicle object from a database record
    if (record.vehicleType == "Plane")
    {
        return std::make_shared<Plane>(
            record.id,
            record.airline,
            record.flightNumber,
            record.flightNo,
            record.capacity);
    }
    else if (record.vehicleType == "Cab")
    {
        return std::make_shared<Cab>(
            record.id,
            record.licensePlate,
            record.driverName,
            record.capacity);
    }
    else if (record.vehicleType == "Train")
    {
        return std::make_shared<Train>(
            record.id,
            record.trainNumber,
            record.platform,
            record.capacity);
    }

    return nullptr; // Unknown vehicle type
}