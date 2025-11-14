# Ticket Booking System

A console-based ticket booking system built in C++ for COMP 3140. This application allows customers to book transportation tickets and provides administrative functions for managing the system.

## Features

### Customer Features

- User registration and login
- Browse available tickets
- Book tickets
- View booked tickets
- Cancel ticket bookings

### Admin Features

- View all ticket bookings in the system
- Manage tickets (add, edit, delete)
- View customer list
- Generate reports

## Project Structure

```
comp3140/
├── include/                    # Header files
│   ├── BaseUser.h             # Base user class
│   ├── Admin.h                # Admin entity (inherits from BaseUser)
│   ├── Customer.h             # Customer entity (inherits from BaseUser)
│   ├── BookingService.h       # Booking business logic
│   ├── TicketService.h        # Ticket management logic
│   └── AdminService.h         # Administrative operations logic
├── src/                       # Source files
│   ├── BaseUser.cpp           # Base user implementation
│   ├── Admin.cpp              # Admin entity implementation
│   ├── Customer.cpp           # Customer entity implementation
│   ├── BookingService.cpp     # Booking service implementation
│   ├── TicketService.cpp      # Ticket service implementation
│   ├── AdminService.cpp       # Admin service implementation
│   └── main.cpp               # Main entry point and controller
├── obj/                       # Object files (generated during build)
├── Makefile                   # Build configuration
├── README.md                  # This file
```

## Architecture

The system follows a **service-oriented architecture** with clear separation of concerns:

### Entity Layer (Domain Models)

- **BaseUser**: Base class containing common user properties (id, username, password, fullName, email)
  - **Admin**: Inherits from BaseUser, manages admin state (adminLevel)
  - **Customer**: Inherits from BaseUser, manages customer state (phone, bookingIds)

### Service Layer (Business Logic)

- **BookingService**: Handles booking creation, cancellation, validation, and refund calculations
- **TicketService**: Manages ticket CRUD operations, availability, and browsing
- **AdminService**: Provides administrative operations, reports, and permission validation

### Controller Layer

- **main.cpp**: Manages user interactions between services and entities

This architecture ensures:

- ✅ **Entities** focus only on state management
- ✅ **Services** contain all business logic and rules
- ✅ **Controllers** orchestrate the flow between layers

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
