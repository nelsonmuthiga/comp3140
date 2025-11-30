# Ticket Booking System

A console-based ticket booking system built in C++ for COMP 3140. This application allows customers to book transportation tickets (Cab, Plane, Train) and provides administrative functions for managing the system.

## Features

### Customer Features

- ✅ User registration and login with database persistence
- ✅ Browse available tickets
- ✅ Book tickets with automatic availability tracking
- ✅ View booked tickets
- ✅ Cancel ticket bookings with refund calculation

### Admin Features

- ✅ View all ticket bookings in the system
- ✅ Manage tickets (add, edit, delete)
- ✅ View customer list
- ✅ Generate reports (Sales, Customer Activity, Booking Statistics)

### Database Features

- ✅ SQLite3 integration for persistent storage
- ✅ Automatic database initialization and table creation
- ✅ Seed data for default tickets and admin user

## Quick Start

```bash
# Build the project
make

# Run the application
./ticket_system      # Linux/Mac
ticket_system.exe    # Windows
```

### Default Login Credentials

| Role     | Username               | Password |
| -------- | ---------------------- | -------- |
| Admin    | `admin`                | `admin`  |
| Customer | Register a new account |

## Documentation

- [User Guide](docs/USER_GUIDE.md) - How to use the application
- [Database Schema](docs/DATABASE.md) - Database structure and queries
- [Architecture](docs/ARCHITECTURE.md) - System design and code structure

## Project Structure

```
comp3140/
├── include/                    # Header files
│   ├── BaseUser.h             # Base user class
│   ├── Admin.h                # Admin entity
│   ├── Customer.h             # Customer entity
│   ├── Database.h             # SQLite database wrapper
│   ├── BookingService.h       # Booking business logic
│   ├── TicketService.h        # Ticket management logic
│   ├── AdminService.h         # Administrative operations
│   ├── sqlite3.h              # SQLite3 header
│   └── sqlite3.c              # SQLite3 amalgamation
├── src/                       # Source files
│   ├── main.cpp               # Entry point and UI controller
│   ├── Database.cpp           # Database implementation
│   ├── BaseUser.cpp           # Base user implementation
│   ├── Admin.cpp              # Admin implementation
│   ├── Customer.cpp           # Customer implementation
│   ├── BookingService.cpp     # Booking service implementation
│   ├── TicketService.cpp      # Ticket service implementation
│   └── AdminService.cpp       # Admin service implementation
├── obj/                       # Compiled object files
├── docs/                      # Documentation
├── ticketsystem.db            # SQLite database (auto-generated)
├── Makefile                   # Build configuration
└── README.md                  # This file
```

## Requirements

- C++ compiler with C++17 support (g++ recommended)
- Make build system
- No external dependencies (SQLite3 is bundled)

## Building

```bash
make            # Build the project
make clean      # Remove build files
make rebuild    # Clean and rebuild
make run        # Build and run
```

## Technology Stack

- **Language**: C++17
- **Database**: SQLite3 (embedded)
- **Build System**: Make
- **Architecture**: Service-oriented with MVC pattern

## Authors

Nelson Muthiga & Utsav Shakya

## Course

COMP 3140
