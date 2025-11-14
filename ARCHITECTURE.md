# Architecture Diagram - Ticket Booking System

## Layer Separation

```
┌─────────────────────────────────────────────────────────────────────┐
│                      PRESENTATION LAYER                              │
│                         (main.cpp)                                   │
│  - Handles user input/output                                        │
│  - Menu displays and navigation                                     │
│  - Coordinates services and entities                                │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│                      SERVICE LAYER                                   │
│                    (Business Logic)                                  │
│                                                                      │
│  ┌───────────────────┐  ┌───────────────────┐  ┌──────────────┐   │
│  │ BookingService    │  │ TicketService     │  │AdminService  │   │
│  ├───────────────────┤  ├───────────────────┤  ├──────────────┤   │
│  │• createBooking()  │  │• getAvailable     │  │• viewAll     │   │
│  │• cancelBooking()  │  │  Tickets()        │  │  Customers() │   │
│  │• getAllBookings() │  │• createTicket()   │  │• generate    │   │
│  │• validateBooking()│  │• modifyTicket()   │  │  Reports()   │   │
│  │                   │  │• deleteTicket()   │  │• validate    │   │
│  │Business Rules:    │  │• display          │  │  AdminAccess()│   │
│  │- Availability     │  │  Available        │  │              │   │
│  │- Refund policy    │  │  Tickets()        │  │Permissions:  │   │
│  │- Ownership        │  │                   │  │- Super admin │   │
│  │  verification     │  │Validation:        │  │- Regular     │   │
│  │                   │  │- Price > 0        │  │  admin       │   │
│  │                   │  │- Availability ≥ 0 │  │              │   │
│  └───────────────────┘  └───────────────────┘  └──────────────┘   │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│                      DOMAIN LAYER                                    │
│                    (Entity Classes)                                  │
│                                                                      │
│  ┌─────────────────────┐        ┌─────────────────────┐           │
│  │   Customer          │        │   Admin             │           │
│  ├─────────────────────┤        ├─────────────────────┤           │
│  │ State:              │        │ State:              │           │
│  │ - id                │        │ - id                │           │
│  │ - userName          │        │ - userName          │           │
│  │ - password          │        │ - password          │           │
│  │ - fullName          │        │ - fullName          │           │
│  │ - email             │        │ - email             │           │
│  │ - phone             │        │ - adminLevel        │           │
│  │ - bookingIds[]      │        │                     │           │
│  │                     │        │                     │           │
│  │ Methods:            │        │ Methods:            │           │
│  │ + getPhone()        │        │ + getAdminLevel()   │           │
│  │ + setPhone()        │        │ + setAdminLevel()   │           │
│  │ + getBookingIds()   │        │                     │           │
│  │ + addBookingId()    │        │                     │           │
│  │ + removeBookingId() │        │                     │           │
│  └─────────────────────┘        └─────────────────────┘           │
│             │                              │                        │
│             └──────────────┬───────────────┘                        │
│                            │                                        │
│                  ┌─────────▼────────┐                              │
│                  │   BaseUser       │                              │
│                  ├──────────────────┤                              │
│                  │ Common State:    │                              │
│                  │ - id             │                              │
│                  │ - userName       │                              │
│                  │ - password       │                              │
│                  │ - fullName       │                              │
│                  │ - email          │                              │
│                  │                  │                              │
│                  │ Methods:         │                              │
│                  │ + login()        │                              │
│                  │ + logout()       │                              │
│                  └──────────────────┘                              │
└─────────────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│                   PERSISTENCE LAYER (Future)                         │
│                      (To be implemented)                             │
│                                                                      │
│  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐ │
│  │ BookingRepository│  │ TicketRepository │  │ UserRepository   │ │
│  └──────────────────┘  └──────────────────┘  └──────────────────┘ │
└─────────────────────────────────────────────────────────────────────┘
```

## Data Flow Examples

### Customer Books a Ticket

```
User Input (main.cpp)
    │
    ├─► Get available tickets from TicketService
    │   └─► Display to user
    │
    ├─► User selects ticket type and quantity
    │
    ├─► Call BookingService.createBooking(customer, type, qty)
    │   │
    │   ├─► Validate availability
    │   ├─► Apply business rules
    │   ├─► Generate booking ID
    │   └─► Return booking ID
    │
    └─► Call customer.addBookingId(bookingId)
        └─► Update customer state
```

### Admin Generates Report

```
User Input (main.cpp)
    │
    ├─► Display AdminService.displayReportsMenu()
    │
    ├─► User selects report type
    │
    └─► Call AdminService.generateSalesReport()
        │
        ├─► Calculate total revenue
        ├─► Aggregate booking statistics
        ├─► Format report data
        └─► Return formatted report string
```

### Customer Cancels Booking

```
User Input (main.cpp)
    │
    ├─► Get booking ID from user
    │
    ├─► Call BookingService.cancelBooking(bookingId, customer)
    │   │
    │   ├─► Verify booking ownership
    │   ├─► Check cancellation policy
    │   ├─► Calculate refund
    │   └─► Return success/failure
    │
    └─► If successful:
        └─► Call customer.removeBookingId(bookingId)
            └─► Update customer state
```

## Key Design Principles Applied

### 1. Separation of Concerns

- **Presentation**: User interaction (main.cpp)
- **Business Logic**: Services (BookingService, TicketService, AdminService)
- **Domain**: Entities (Customer, Admin, BaseUser)
- **Persistence**: To be added (Repositories)

### 2. Single Responsibility Principle

Each class has one reason to change:

- Entities change when domain model changes
- Services change when business rules change
- Controllers change when UI/UX changes

### 3. Dependency Direction

```
Presentation → Services → Entities
```

Dependencies flow downward, never upward.

### 4. Open/Closed Principle

- Services are open for extension (new operations)
- Entities are closed for modification (stable state)

## Benefits Summary

✅ **Testability**: Services can be unit tested independently
✅ **Maintainability**: Clear responsibility boundaries
✅ **Extensibility**: Easy to add new features
✅ **Reusability**: Services can be reused across different UIs
✅ **Modularity**: Changes are localized to specific layers
✅ **Readability**: Clear separation makes code easier to understand
