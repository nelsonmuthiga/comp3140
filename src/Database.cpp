#include "Database.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

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
            type TEXT UNIQUE NOT NULL,
            description TEXT NOT NULL,
            price REAL NOT NULL CHECK(price > 0),
            availability INTEGER NOT NULL DEFAULT 0 CHECK(availability >= 0),
            date TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
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

    if (!executeQuery(userTableSQL))
        return false;
    if (!executeQuery(ticketsTableSQL))
        return false;
    if (!executeQuery(bookingsTableSQL))
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

    // Check if tickets exist
    std::vector<TicketInfo> tickets = getAllTickets();
    if (tickets.empty())
    {
        createTicket({"Cab", "City Taxi Service", 25.0, 50, "Available Now"});
        createTicket({"Plane", "Flight to New York", 450.0, 20, "2025-12-01"});
        createTicket({"Train", "Express to Boston", 85.0, 100, "2025-11-25"});
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
    const char *sql = "SELECT type, description, price, availability, date FROM tickets;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return tickets;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        TicketInfo ticket;
        ticket.type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        ticket.description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        ticket.price = sqlite3_column_double(stmt, 2);
        ticket.availability = sqlite3_column_int(stmt, 3);
        const char *date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        ticket.date = date ? date : "";
        tickets.push_back(ticket);
    }

    sqlite3_finalize(stmt);
    return tickets;
}

TicketInfo Database::getTicketByType(const std::string &type)
{
    TicketInfo ticket = {"", "", 0.0, 0, ""};
    const char *sql = "SELECT type, description, price, availability, date FROM tickets WHERE type = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return ticket;
    }

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        ticket.type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        ticket.description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        ticket.price = sqlite3_column_double(stmt, 2);
        ticket.availability = sqlite3_column_int(stmt, 3);
        const char *date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        ticket.date = date ? date : "";
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
    const char *sql = "INSERT INTO tickets (type, description, price, availability, date) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, ticket.type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, ticket.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, ticket.price);
    sqlite3_bind_int(stmt, 4, ticket.availability);
    sqlite3_bind_text(stmt, 5, ticket.date.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return success;
}

bool Database::updateTicketAvailability(const std::string &type, int change)
{
    const char *sql = "UPDATE tickets SET availability = availability + ? WHERE type = ? AND availability + ? >= 0;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, change);
    sqlite3_bind_text(stmt, 2, type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, change);

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