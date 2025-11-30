# Database Schema

The Ticket Booking System uses SQLite3 for persistent data storage. The database file `ticketsystem.db` is automatically created in the project root directory on first run.

## Tables

### Users Table

Stores all registered users (both customers and admins).

```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    full_name TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    phone TEXT,
    user_type TEXT NOT NULL CHECK(user_type IN ('admin', 'customer')),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

| Column     | Type     | Description                         |
| ---------- | -------- | ----------------------------------- |
| id         | INTEGER  | Auto-incrementing primary key       |
| username   | TEXT     | Unique login username               |
| password   | TEXT     | User password (plain text for demo) |
| full_name  | TEXT     | User's display name                 |
| email      | TEXT     | Unique email address                |
| phone      | TEXT     | Optional phone number               |
| user_type  | TEXT     | Either 'admin' or 'customer'        |
| created_at | DATETIME | Account creation timestamp          |

### Tickets Table

Stores available ticket types for booking.

```sql
CREATE TABLE tickets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT UNIQUE NOT NULL,
    description TEXT NOT NULL,
    price REAL NOT NULL CHECK(price > 0),
    availability INTEGER NOT NULL DEFAULT 0 CHECK(availability >= 0),
    date TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

| Column       | Type     | Description                        |
| ------------ | -------- | ---------------------------------- |
| id           | INTEGER  | Auto-incrementing primary key      |
| type         | TEXT     | Unique ticket type (e.g., "Plane") |
| description  | TEXT     | Ticket description                 |
| price        | REAL     | Price per ticket (must be > 0)     |
| availability | INTEGER  | Number of tickets available        |
| date         | TEXT     | Travel date or "Available Now"     |
| created_at   | DATETIME | Record creation timestamp          |

### Bookings Table

Stores all customer bookings.

```sql
CREATE TABLE bookings (
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
```

| Column      | Type     | Description                             |
| ----------- | -------- | --------------------------------------- |
| id          | INTEGER  | Auto-incrementing primary key           |
| booking_id  | TEXT     | Unique booking reference (e.g., BK1234) |
| user_id     | INTEGER  | Foreign key to users table              |
| ticket_id   | INTEGER  | Foreign key to tickets table            |
| ticket_type | TEXT     | Ticket type name (denormalized)         |
| num_tickets | INTEGER  | Number of tickets booked                |
| total_price | REAL     | Total price (price × num_tickets)       |
| status      | TEXT     | 'active' or 'cancelled'                 |
| created_at  | DATETIME | Booking creation timestamp              |

## Seed Data

On first initialization, the database is seeded with:

### Default Admin User

```sql
INSERT INTO users (username, password, full_name, email, phone, user_type)
VALUES ('admin', 'admin', 'Administrator', 'admin@ticketsystem.com', '', 'admin');
```

### Default Tickets

```sql
INSERT INTO tickets (type, description, price, availability, date) VALUES
('Cab', 'City Taxi Service', 25.0, 50, 'Available Now'),
('Plane', 'Flight to New York', 450.0, 20, '2025-12-01'),
('Train', 'Express to Boston', 85.0, 100, '2025-11-25');
```

## Database Operations

### User Operations

| Method                | Description                    |
| --------------------- | ------------------------------ |
| `createUser()`        | Register a new user            |
| `getUserByUsername()` | Authenticate/retrieve user     |
| `getUserById()`       | Get user by ID                 |
| `getAllCustomers()`   | List all customers (for admin) |
| `userExists()`        | Check if username is taken     |

### Ticket Operations

| Method                       | Description                 |
| ---------------------------- | --------------------------- |
| `getAllTickets()`            | Get all available tickets   |
| `getTicketByType()`          | Get specific ticket details |
| `getTicketIdByType()`        | Get ticket ID for booking   |
| `createTicket()`             | Add a new ticket type       |
| `updateTicketAvailability()` | Adjust availability (+/-)   |
| `deleteTicket()`             | Remove a ticket type        |
| `ticketExists()`             | Check if ticket type exists |

### Booking Operations

| Method                  | Description                      |
| ----------------------- | -------------------------------- |
| `createBooking()`       | Create a new booking             |
| `cancelBooking()`       | Cancel (set status to cancelled) |
| `getBookingsByUserId()` | Get customer's bookings          |
| `getAllBookings()`      | Get all bookings (for admin)     |
| `getBookingById()`      | Get specific booking             |
| `bookingExists()`       | Check if booking ID exists       |

### Report Operations

| Method                   | Description                  |
| ------------------------ | ---------------------------- |
| `getTotalBookings()`     | Count all bookings           |
| `getActiveBookings()`    | Count active bookings        |
| `getCancelledBookings()` | Count cancelled bookings     |
| `getTotalRevenue()`      | Sum of active booking prices |
| `getTotalCustomers()`    | Count registered customers   |

## Entity Relationship Diagram

```
┌─────────────┐       ┌─────────────┐       ┌─────────────┐
│    users    │       │  bookings   │       │   tickets   │
├─────────────┤       ├─────────────┤       ├─────────────┤
│ id (PK)     │◄──────│ user_id (FK)│       │ id (PK)     │
│ username    │       │ ticket_id(FK)│──────►│ type        │
│ password    │       │ booking_id  │       │ description │
│ full_name   │       │ ticket_type │       │ price       │
│ email       │       │ num_tickets │       │ availability│
│ phone       │       │ total_price │       │ date        │
│ user_type   │       │ status      │       │ created_at  │
│ created_at  │       │ created_at  │       └─────────────┘
└─────────────┘       └─────────────┘
```

## Viewing the Database

You can inspect the database using any SQLite browser:

- [DB Browser for SQLite](https://sqlitebrowser.org/) (GUI)
- Command line: `sqlite3 ticketsystem.db`

```bash
# Example queries
sqlite3 ticketsystem.db "SELECT * FROM users;"
sqlite3 ticketsystem.db "SELECT * FROM tickets;"
sqlite3 ticketsystem.db "SELECT * FROM bookings;"
```
