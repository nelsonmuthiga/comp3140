# Code Comparison: Before vs After Refactoring

## Customer Class

### BEFORE (Entity with Business Logic)

```cpp
// Customer.h
class Customer : public BaseUser {
    // ... state fields ...
public:
    void bookTicket(const std::string &bookingId);
    void viewMyTickets() const;
    void cancelTicket(const std::string &bookingId);
    void browseEvents() const;
};

// Customer.cpp
void Customer::bookTicket(const std::string &bookingId) {
    bookingIds.push_back(bookingId);
    std::cout << "Ticket booked successfully! Booking ID: " << bookingId << std::endl;
    // Business logic mixed with entity
}

void Customer::cancelTicket(const std::string &bookingId) {
    auto it = std::find(bookingIds.begin(), bookingIds.end(), bookingId);
    if (it != bookingIds.end()) {
        bookingIds.erase(it);
        std::cout << "Ticket cancellation successful..." << std::endl;
        std::cout << "Refund will be processed within 7 business days." << std::endl;
    }
    // Business logic for refunds in entity!
}
```

### AFTER (Thin Entity)

```cpp
// Customer.h
class Customer : public BaseUser {
    // ... state fields ...
public:
    // Only state management - no business logic
    std::string getPhone() const;
    void setPhone(const std::string &phone);
    std::vector<std::string> getBookingIds() const;
    void addBookingId(const std::string &bookingId);
    void removeBookingId(const std::string &bookingId);
};

// Customer.cpp
void Customer::addBookingId(const std::string &bookingId) {
    bookingIds.push_back(bookingId);  // Simple state change only
}

void Customer::removeBookingId(const std::string &bookingId) {
    auto it = std::find(bookingIds.begin(), bookingIds.end(), bookingId);
    if (it != bookingIds.end()) {
        bookingIds.erase(it);  // Simple state change only
    }
}
```

---

## Admin Class

### BEFORE (Entity with Business Logic)

```cpp
// Admin.h
class Admin : public BaseUser {
    // ... state fields ...
public:
    void viewAllTickets() const;
    void manageBookings() const;
    void viewCustomerList() const;
    void generateReports() const;
};

// Admin.cpp
void Admin::viewAllTickets() const {
    std::cout << "--- All Tickets (Admin View) ---" << std::endl;
    std::cout << "All system bookings:" << std::endl;
    // Hardcoded display logic in entity
}

void Admin::generateReports() const {
    std::cout << "--- Generate Reports ---" << std::endl;
    std::cout << "Reporting feature coming soon..." << std::endl;
    // Business logic in entity
}
```

### AFTER (Thin Entity)

```cpp
// Admin.h
class Admin : public BaseUser {
    // ... state fields ...
public:
    // Only state management - no business logic
    std::string getAdminLevel() const;
    void setAdminLevel(const std::string &level);
};

// Admin.cpp
std::string Admin::getAdminLevel() const {
    return adminLevel;  // Simple getter only
}

void Admin::setAdminLevel(const std::string &level) {
    this->adminLevel = level;  // Simple setter only
}
```

---

## New Service Classes

### BookingService (NEW)

```cpp
// BookingService.h
class BookingService {
public:
    std::string createBooking(std::shared_ptr<Customer> customer,
                             const std::string& ticketType,
                             int numTickets);
    bool cancelBooking(const std::string& bookingId,
                      std::shared_ptr<Customer> customer);
    std::vector<std::string> getCustomerBookings(int customerId) const;
    std::vector<std::string> getAllBookings() const;

private:
    bool isTicketAvailable(const std::string& ticketType, int quantity) const;
    bool canCancelBooking(const std::string& bookingId) const;
    double calculateRefund(const std::string& bookingId) const;
    std::string generateBookingId() const;
};

// BookingService.cpp
std::string BookingService::createBooking(...) {
    // Validate inputs
    if (!customer || numTickets <= 0) {
        return "";
    }

    // Check availability (business rule)
    if (!isTicketAvailable(ticketType, numTickets)) {
        return "";
    }

    // Generate booking ID
    std::string bookingId = generateBookingId();

    // All business logic is here, not in Customer!
    return bookingId;
}

bool BookingService::cancelBooking(...) {
    // Validate ownership (business rule)
    auto bookings = customer->getBookingIds();
    if (std::find(bookings.begin(), bookings.end(), bookingId) == bookings.end()) {
        return false;
    }

    // Check cancellation policy (business rule)
    if (!canCancelBooking(bookingId)) {
        return false;
    }

    // Calculate refund (business rule)
    double refundAmount = calculateRefund(bookingId);

    return true;
}
```

### TicketService (NEW)

```cpp
// TicketService.h
class TicketService {
public:
    std::vector<TicketInfo> getAvailableTickets() const;
    TicketInfo getTicketDetails(const std::string& ticketType) const;
    bool createTicket(const TicketInfo& ticket);
    bool modifyTicket(const std::string& ticketType, const TicketInfo& newInfo);
    bool deleteTicket(const std::string& ticketType);
    void displayAvailableTickets() const;

private:
    bool validateTicketInfo(const TicketInfo& ticket) const;
    bool ticketExists(const std::string& ticketType) const;
};

// TicketService.cpp
bool TicketService::createTicket(const TicketInfo& ticket) {
    // Business validation
    if (!validateTicketInfo(ticket)) {
        return false;
    }

    // Business rule: no duplicate types
    if (ticketExists(ticket.type)) {
        return false;
    }

    // Business logic for ticket creation
    return true;
}
```

### AdminService (NEW)

```cpp
// AdminService.h
class AdminService {
public:
    std::vector<std::string> viewAllCustomers() const;
    std::string generateSalesReport() const;
    std::string generateCustomerReport() const;
    std::string generateBookingStatistics() const;
    bool validateAdminAccess(std::shared_ptr<Admin> admin,
                            const std::string& operation) const;

private:
    bool hasPermission(const std::string& adminLevel,
                      const std::string& operation) const;
    double calculateTotalRevenue() const;
};

// AdminService.cpp
std::string AdminService::generateSalesReport() const {
    std::ostringstream report;

    // Business logic for report generation
    double totalRevenue = calculateTotalRevenue();
    int totalBookings = 45;

    report << "Total Revenue: $" << totalRevenue << "\n";
    report << "Total Bookings: " << totalBookings << "\n";

    return report.str();
}

bool AdminService::validateAdminAccess(...) {
    // Business rule: permission checking
    return hasPermission(admin->getAdminLevel(), operation);
}
```

---

## Main Controller

### BEFORE (Direct Entity Calls)

```cpp
// main.cpp - Customer menu
case 1: {
    customer->browseEvents();  // Entity doing business logic
    // ... get input ...
    customer->bookTicket(bookingId);  // Entity doing business logic
    break;
}
case 3: {
    customer->cancelTicket(bookingId);  // Entity doing business logic
    break;
}

// main.cpp - Admin menu
case 1:
    admin->viewAllTickets();  // Entity doing business logic
    break;
case 4:
    admin->generateReports();  // Entity doing business logic
    break;
```

### AFTER (Service Coordination)

```cpp
// main.cpp - Customer menu
case 1: {
    // Service handles display
    auto tickets = ticketService.getAvailableTickets();
    // Display tickets...

    // Service handles business logic
    string bookingId = bookingService.createBooking(
        customer, tickets[choice].type, numTickets
    );

    // Entity only updates state
    if (!bookingId.empty()) {
        customer->addBookingId(bookingId);
    }
    break;
}
case 3: {
    // Service handles business logic
    if (bookingService.cancelBooking(bookingId, customer)) {
        // Entity only updates state
        customer->removeBookingId(bookingId);
    }
    break;
}

// main.cpp - Admin menu
case 1: {
    // Service handles business logic
    auto allBookings = bookingService.getAllBookings();
    // Display bookings...
    break;
}
case 4: {
    // Service handles business logic
    cout << adminService.generateSalesReport();
    break;
}
```

---

## Key Differences Summary

| Aspect              | Before                       | After                    |
| ------------------- | ---------------------------- | ------------------------ |
| **Entities**        | Fat (state + business logic) | Thin (state only)        |
| **Business Logic**  | Scattered in entities        | Centralized in services  |
| **Testability**     | Hard (coupled to UI)         | Easy (isolated services) |
| **Reusability**     | Low (tied to entities)       | High (service methods)   |
| **Maintainability** | Poor (tangled concerns)      | Good (clear separation)  |
| **Extensibility**   | Difficult (modify entities)  | Easy (add services)      |

## Lines of Code Comparison

### Before

- `Customer.cpp`: ~80 lines (state + business logic)
- `Admin.cpp`: ~70 lines (state + business logic)
- Total entity code: ~150 lines

### After

- `Customer.cpp`: ~30 lines (state only)
- `Admin.cpp`: ~15 lines (state only)
- `BookingService.cpp`: ~160 lines (business logic)
- `TicketService.cpp`: ~140 lines (business logic)
- `AdminService.cpp`: ~120 lines (business logic)
- Total: ~465 lines

**Result**: More total code, but much better organized with clear responsibilities!

## Maintainability Example

### Changing Refund Policy

**BEFORE**: Would need to modify `Customer.cpp` (entity class)

```cpp
// In Customer.cpp - coupled to entity
void Customer::cancelTicket(...) {
    // Refund logic hardcoded in entity
    std::cout << "Refund will be processed within 7 business days." << std::endl;
}
```

**AFTER**: Only need to modify `BookingService.cpp` (service class)

```cpp
// In BookingService.cpp - isolated business logic
double BookingService::calculateRefund(...) const {
    // Easy to change refund calculation
    // Doesn't affect entity classes at all
    double refundPercentage = 0.80;  // Can easily change to 0.90
    return bookingAmount * refundPercentage;
}
```

✅ Entities remain stable, business logic is flexible!
