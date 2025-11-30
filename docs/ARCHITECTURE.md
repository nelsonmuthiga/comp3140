# System Architecture

This document describes the architecture and design patterns used in the Ticket Booking System.

## Overview

The system follows a **layered architecture** with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────┐
│                    Presentation Layer                    │
│                      (main.cpp)                          │
│              Console UI / Menu Navigation                │
├─────────────────────────────────────────────────────────┤
│                     Service Layer                        │
│    ┌─────────────┐ ┌──────────────┐ ┌──────────────┐   │
│    │ AdminService│ │BookingService│ │TicketService │   │
│    └─────────────┘ └──────────────┘ └──────────────┘   │
│              Business Logic & Validation                 │
├─────────────────────────────────────────────────────────┤
│                      Entity Layer                        │
│    ┌──────────┐    ┌──────────┐    ┌──────────┐        │
│    │ BaseUser │    │  Admin   │    │ Customer │        │
│    └──────────┘    └──────────┘    └──────────┘        │
│                   Domain Models                          │
├─────────────────────────────────────────────────────────┤
│                    Data Access Layer                     │
│                     (Database.cpp)                       │
│                 SQLite3 Operations                       │
└─────────────────────────────────────────────────────────┘
```

## Design Patterns

### 1. Singleton Pattern

**Used in:** `Database` class

The Database class implements the Singleton pattern to ensure only one database connection exists throughout the application lifecycle.

```cpp
class Database {
public:
    static Database& getInstance();  // Returns single instance
private:
    Database() : db(nullptr) {}      // Private constructor
    static Database* instance;
};
```

**Usage:**

```cpp
Database::getInstance().initialize("ticketsystem.db");
Database::getInstance().getAllTickets();
```

### 2. Inheritance

**Used in:** Entity classes

`BaseUser` serves as the base class with common properties, while `Admin` and `Customer` extend it with role-specific functionality.

```
        ┌──────────────┐
        │   BaseUser   │
        ├──────────────┤
        │ - id         │
        │ - userName   │
        │ - password   │
        │ - fullName   │
        │ - email      │
        └──────┬───────┘
               │
       ┌───────┴───────┐
       ▼               ▼
┌──────────┐    ┌──────────┐
│  Admin   │    │ Customer │
├──────────┤    ├──────────┤
│          │    │ - phone  │
│          │    │ - booking│
│          │    │   Ids    │
└──────────┘    └──────────┘
```

### 3. Service Layer Pattern

**Used in:** All service classes

Business logic is encapsulated in service classes, separating it from both the UI (main.cpp) and data access (Database).

| Service          | Responsibility                                 |
| ---------------- | ---------------------------------------------- |
| `BookingService` | Create/cancel bookings, refund calculation     |
| `TicketService`  | CRUD operations for tickets, availability      |
| `AdminService`   | Reports, customer management, admin operations |

### 4. Smart Pointers

**Used in:** main.cpp for user objects

The application uses `std::shared_ptr` for managing user objects to ensure proper memory management.

```cpp
shared_ptr<BaseUser> currentUser = nullptr;
shared_ptr<Customer> customer = dynamic_pointer_cast<Customer>(currentUser);
```

## Component Details

### Presentation Layer (main.cpp)

Responsibilities:

- Display menus and prompts
- Handle user input
- Navigate between screens
- Coordinate between services

Key functions:

```cpp
void displayMainMenu();
void displayCustomerMenu();
void displayAdminMenu();
void handleMainMenu();
void handleCustomerMenu(shared_ptr<Customer> customer);
void handleAdminMenu(shared_ptr<Admin> admin);
void login();
void registerUser();
```

### Service Layer

#### BookingService

```cpp
class BookingService {
public:
    string createBooking(shared_ptr<Customer>, string ticketType, int numTickets);
    bool cancelBooking(string bookingId, shared_ptr<Customer>);
    vector<string> getCustomerBookings(int customerId);
    vector<string> getAllBookings();
private:
    bool isTicketAvailable(string ticketType, int quantity);
    bool canCancelBooking(string bookingId);
    double calculateRefund(string bookingId);
    string generateBookingId();
};
```

#### TicketService

```cpp
class TicketService {
public:
    vector<TicketInfo> getAvailableTickets();
    TicketInfo getTicketDetails(string ticketType);
    bool updateTicketAvailability(string ticketType, int change);
    bool createTicket(TicketInfo ticket);
    bool modifyTicket(string ticketType, TicketInfo newInfo);
    bool deleteTicket(string ticketType);
    void displayAvailableTickets();
private:
    bool validateTicketInfo(TicketInfo ticket);
    bool ticketExists(string ticketType);
};
```

#### AdminService

```cpp
class AdminService {
public:
    vector<string> viewAllCustomers();
    string generateSalesReport();
    string generateCustomerReport();
    string generateBookingStatistics();
    void displayCustomerList();
    void displayReportsMenu();
private:
    double calculateTotalRevenue();
};
```

### Data Access Layer (Database)

The Database class provides a clean interface for all SQLite operations:

```cpp
class Database {
public:
    static Database& getInstance();
    bool initialize(string dbPath = "database.db");
    void close();

    // User operations
    bool createUser(...);
    UserRecord getUserByUsername(string username);

    // Ticket operations
    vector<TicketInfo> getAllTickets();
    bool createTicket(TicketInfo ticket);

    // Booking operations
    string createBooking(...);
    bool cancelBooking(string bookingId);

    // Report operations
    double getTotalRevenue();
    int getTotalBookings();
};
```

## Data Flow Examples

### Booking a Ticket

```
1. User selects "Book Ticket" in Customer Menu
           │
           ▼
2. main.cpp calls ticketService.getAvailableTickets()
           │
           ▼
3. TicketService calls Database::getInstance().getAllTickets()
           │
           ▼
4. User selects ticket type and quantity
           │
           ▼
5. main.cpp calls bookingService.createBooking(customer, type, qty)
           │
           ▼
6. BookingService:
   - Gets ticket info from Database
   - Checks availability
   - Calculates total price
   - Creates booking in Database
   - Updates ticket availability in Database
           │
           ▼
7. Returns booking ID to main.cpp
           │
           ▼
8. main.cpp displays confirmation to user
```

### Generating Sales Report

```
1. Admin selects "Generate Reports" → "Sales Report"
           │
           ▼
2. main.cpp calls adminService.generateSalesReport()
           │
           ▼
3. AdminService calls:
   - Database::getInstance().getTotalRevenue()
   - Database::getInstance().getTotalBookings()
   - Database::getInstance().getActiveBookings()
   - Database::getInstance().getCancelledBookings()
   - Database::getInstance().getAllBookings()
           │
           ▼
4. AdminService formats data into report string
           │
           ▼
5. Returns formatted report to main.cpp
           │
           ▼
6. main.cpp displays report to admin
```

## File Dependencies

```
main.cpp
├── Admin.h
├── Customer.h
├── BookingService.h
├── TicketService.h
├── AdminService.h
└── Database.h

BookingService.cpp
├── BookingService.h
├── Customer.h
└── Database.h

TicketService.cpp
├── TicketService.h
└── Database.h

AdminService.cpp
├── AdminService.h
├── Admin.h
└── Database.h

Database.cpp
├── Database.h
├── TicketService.h (for TicketInfo struct)
└── sqlite3.h
```

## Build Process

The Makefile compiles:

1. All `.cpp` files in `src/` → `.o` files in `obj/`
2. `sqlite3.c` from `include/` → `sqlite3.o` in `obj/`
3. Links all object files → `ticket_system.exe`

```makefile
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJECTS += $(OBJ_DIR)/sqlite3.o

$(TARGET): $(OBJECTS)
    $(CXX) $(OBJECTS) -o $(TARGET)
```
