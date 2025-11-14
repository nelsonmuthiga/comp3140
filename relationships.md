# UML Class and Relationship Documentation

## Class Table

| **UML Class**                           | **Responsibility**         | **Header File**      | **Source File**        | **Member Functions**                                                                                                                                                     | **Notes**                                                               |
| --------------------------------------- | -------------------------- | -------------------- | ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------- |
| User (BaseUser)                         | Base class for User        | BaseUser.h           | BaseUser.cpp           | login(), logout(), updateInfo(), getId(), getUserName(), getPassword(), getFullName(), getEmail()                                                                        | Abstract base class for all users                                       |
| Admin                                   | Admin entity/identity      | Admin.h              | Admin.cpp              | (inherits BaseUser methods only)                                                                                                                                         | Inherits from BaseUser; simplified - no admin levels, no business logic |
| Customer                                | Customer entity/identity   | Customer.h           | Customer.cpp           | bookTicket(bookingId), viewMyTickets(), cancelTicket(bookingId), browseEvents(), getBookingIds(), addBookingId(), removeBookingId(), getPhoneNumber()                    | Inherits from BaseUser; entity class with booking ID tracking           |
| **AdminService**                        | **Admin business logic**   | **AdminService.h**   | **AdminService.cpp**   | **viewAllCustomers(), generateSalesReport(), generateCustomerReport(), generateBookingStatistics(), validateAdminAccess(), displayCustomerList(), displayReportsMenu()** | **Service class - handles all admin operations and reporting**          |
| **BookingService**                      | **Booking business logic** | **BookingService.h** | **BookingService.cpp** | **createBooking(), cancelBooking(), getCustomerBookings(), getAllBookings(), validateBooking(), calculateRefund()**                                                      | **Service class - handles booking operations**                          |
| **TicketService**                       | **Ticket business logic**  | **TicketService.h**  | **TicketService.cpp**  | **displayAvailableTickets(), getAvailableTickets(), createTicket(), updateTicketAvailability()**                                                                         | **Service class - handles ticket management**                           |
| Vehicle (diff branch, still debugging)  | Base vehicle class         | Vehicle.h            | Vehicle.cpp            | getVehicleId(), getVehicleType(), getVehicleCapacity(), setters                                                                                                          | Abstract base for transport types                                       |
| Airplane (diff branch, still debugging) | Airplane-specific data     | Airplane.h           | Airplane.cpp           | getAirline(), getGateNo(), getFlightNo(), setters                                                                                                                        | Inherits from Vehicle                                                   |
| Train (diff branch, still debugging)    | Train-specific data        | Train.h              | Train.cpp              | getTrainNo(), getPlatform(), setters                                                                                                                                     | Inherits from Vehicle                                                   |
| Cab (diff branch, still debugging)      | Cab-specific data          | Cab.h                | Cab.cpp                | getDriverName(), getLicensePlate(), setters                                                                                                                              | Inherits from Vehicle                                                   |
| Ticket (diff branch, still debugging)   | Booking record             | Ticket.h             | Ticket.cpp             | getTicketId(), getStatus(), getPrice(), getOrigin(), getDestination(), setters                                                                                           | Contains Vehicle and BaseUser references                                |

## Relationships Table

| **Relationship** | **UML Element**               | **Code Representation**                                   | **Notes**                                                                       |
| ---------------- | ----------------------------- | --------------------------------------------------------- | ------------------------------------------------------------------------------- |
| Inheritance      | User → Admin                  | class Admin : public BaseUser                             | "is-a" relationship - Admin is a specialized User (simplified, no admin levels) |
| Inheritance      | User → Customer               | class Customer : public BaseUser                          | "is-a" relationship - Customer is a specialized User                            |
| **Dependency**   | **AdminService → Admin**      | **shared_ptr<Admin> parameter**                           | **Service uses Admin entity for operations**                                    |
| **Dependency**   | **BookingService → Customer** | **shared_ptr<Customer> parameter**                        | **Service uses Customer entity for bookings**                                   |
| **Association**  | **main.cpp → Services**       | **AdminService, BookingService, TicketService instances** | **Main program uses service classes for business logic**                        |
| Inheritance      | Vehicle → Airplane            | class Airplane : public Vehicle                           | Vehicle specialization for air transport                                        |
| Inheritance      | Vehicle → Train               | class Train : public Vehicle                              | Vehicle specialization for rail transport                                       |
| Inheritance      | Vehicle → Cab                 | class Cab : public Vehicle                                | Vehicle specialization for road transport                                       |
| Composition      | Ticket → Vehicle              | Vehicle vehicle; in Ticket class                          | Ticket owns Vehicle information                                                 |
| Association      | Ticket → BaseUser             | BaseUser user; in Ticket class                            | Ticket associated with user who booked it                                       |
| Association      | Customer → Ticket             | vector<string> bookingIds;                                | Customer tracks their booking IDs (no longer aggregation in Admin)              |

## Key Architecture Changes

### Refactoring Summary

1. **Admin** class simplified - removed `adminLevel` member variable and all business logic methods
2. **Service Layer Added** - Created three service classes (AdminService, BookingService, TicketService) following separation of concerns
3. **Entity Classes** - Admin and Customer are now pure entity classes that maintain state only
4. **Business Logic Separation** - All operations moved from entity classes to appropriate service classes
5. **Admin Levels Removed** - Single admin type with full access (simplified from super/regular admin levels)

### Design Pattern

The current architecture follows a **Service-Oriented Architecture** pattern where:

- **Entity classes** (Admin, Customer, BaseUser) handle identity and state
- **Service classes** (AdminService, BookingService, TicketService) handle business logic
- **Main program** orchestrates interaction between entities and services
