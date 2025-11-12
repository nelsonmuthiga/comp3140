# Ticket Booking System

A console-based ticket booking system built in C++ for COMP 3140. This application allows customers to book tickets for events and provides administrative functions for managing the system.

## Features

### Customer Features

- User registration and login
- Browse available events
- Book tickets for events
- View booked tickets
- Cancel ticket bookings

### Admin Features

- View all ticket bookings in the system
- Manage events (add, edit, delete)
- View customer list
- Generate reports

## Project Structure

```
comp3140/
├── include/          # Header files
│   ├── BaseUser.h   # Base user class
│   ├── Admin.h      # Admin class (inherits from BaseUser)
│   └── Customer.h   # Customer class (inherits from BaseUser)
├── src/             # Source files
│   ├── BaseUser.cpp
│   ├── Admin.cpp
│   ├── Customer.cpp
│   └── main.cpp     # Main entry point
├── obj/             # Object files (generated)
├── Makefile         # Build configuration
└── README.md
```

## Class Hierarchy

- **BaseUser**: Base class containing common user properties (id, username, password, fullName, email)
  - **Admin**: Inherits from BaseUser, adds admin-specific functionality
  - **Customer**: Inherits from BaseUser, adds customer-specific functionality (phone, bookingIds)

## Requirements

- C++ compiler with C++11 support (g++)
- Make

## Building the Project

To compile the project, run:

```bash
make
```

Other make commands:

```bash
make rebuild  # Clean and rebuild from scratch
make clean    # Remove build files
make run      # Compile and run the program
```

## Running the Application

After building, run the executable:

```bash
./ticket_system
```

## Usage

### Demo Login Credentials

**Admin Access:**

- Username: `admin`
- Password: `admin`

**Customer Access:**

- Any non-empty username and password combination

### Navigation

The application uses a menu-driven interface. Enter the number corresponding to your desired action and press Enter.

## Development Status

This is currently a skeleton POC (Proof of Concept). The following features are planned for future implementation:

- Database integration for persistent storage
- Actual authentication system
- Real event management

## Author

Nelson Muthiga & Utsav Shakya

## Course

COMP 3140
