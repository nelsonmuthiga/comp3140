# User Guide

This guide explains how to use the Ticket Booking System.

## Getting Started

1. Build and run the application:

   ```bash
   make
   ./ticket_system    # or ticket_system.exe on Windows
   ```

2. You'll see the main menu:

   ```
   ========================================
      Welcome to Ticket Booking System
   ========================================

   --- Main Menu ---
   1. Login
   2. Register
   3. Browse Available Tickets (Guest)
   4. Exit
   ```

---

## Customer Guide

### Registering a New Account

1. Select **2. Register** from the main menu
2. Enter your details:
   - Username (must be unique)
   - Password
   - Full Name
   - Email
   - Phone (optional - press Enter to skip)
3. After successful registration, you can login

### Logging In

1. Select **1. Login** from the main menu
2. Enter your username and password
3. You'll be directed to the Customer Menu

### Browsing Tickets

You can browse tickets without logging in:

1. Select **3. Browse Available Tickets (Guest)**
2. View all available transportation options with prices and availability

### Booking a Ticket

1. Login to your account
2. Select **1. Book Ticket**
3. View the available tickets list
4. Enter the ticket number (1, 2, 3, etc.)
5. Enter the number of tickets you want
6. Confirm your booking

**Example:**

```
--- Available Tickets ---
1. Cab - City Taxi Service (Date: Available Now, Price: $25)
2. Plane - Flight to New York (Date: 2025-12-01, Price: $450)
3. Train - Express to Boston (Date: 2025-11-25, Price: $85)

Select ticket type (1-3): 2
Number of tickets: 2

Processing booking...
Ticket Type: Plane
Number of Tickets: 2
Total Price: $900
Booking ID: BK4523
Booking created successfully!
```

### Viewing Your Tickets

1. Select **2. View My Tickets**
2. See all your current and past bookings with:
   - Booking ID
   - Ticket type
   - Number of tickets
   - Price
   - Status (active/cancelled)

### Cancelling a Booking

1. Select **3. Cancel Ticket**
2. Enter the Booking ID (e.g., BK4523)
3. Receive 80% refund of the total price
4. Refund is processed within 7 business days

---

## Admin Guide

### Logging In as Admin

1. Select **1. Login** from the main menu
2. Enter:
   - Username: `admin`
   - Password: `admin`
3. You'll be directed to the Admin Menu

### Admin Menu Options

```
--- Admin Menu ---
1. View All Tickets
2. Manage Bookings (Add/Edit/Delete)
3. View Customer List
4. Generate Reports
5. Logout
```

### Viewing All Bookings

Select **1. View All Tickets** to see every booking in the system:

```
User: john_doe | Booking ID: BK1234 | Type: Plane | Tickets: 2 | Price: $900 | Status: active
User: jane_smith | Booking ID: BK5678 | Type: Train | Tickets: 1 | Price: $85 | Status: active
```

### Managing Tickets

Select **2. Manage Bookings (Add/Edit/Delete)**:

#### Adding a New Ticket

1. Select **1. Add New Ticket**
2. Enter:
   - Ticket type (e.g., "Bus")
   - Description (e.g., "Express Bus to Chicago")
   - Price (e.g., 55)
   - Availability (e.g., 30)
   - Date (e.g., "2025-12-15")

#### Editing a Ticket

1. Select **2. Edit Ticket**
2. View current tickets
3. Enter the ticket type to edit
4. Enter new values for all fields

#### Deleting a Ticket

1. Select **3. Delete Ticket**
2. View current tickets
3. Enter the ticket type to delete
4. Confirm with 'y'

### Viewing Customer List

Select **3. View Customer List** to see all registered customers:

```
ID: 2 | Name: John Doe | Email: john@example.com | Phone: 555-1234
ID: 3 | Name: Jane Smith | Email: jane@example.com | Phone: 555-5678
```

### Generating Reports

Select **4. Generate Reports**:

#### 1. Sales Report

Shows:

- Total revenue from active bookings
- Total/active/cancelled booking counts
- Revenue breakdown by ticket type

#### 2. Customer Activity Report

Shows:

- Total registered customers
- Top 5 customers by number of bookings

#### 3. Booking Statistics

Shows:

- Average booking value
- Average tickets per booking
- Cancellation rate percentage

---

## Tips

- **Booking IDs** always start with "BK" followed by numbers (e.g., BK1234)
- **Phone numbers** are optional during registration
- **Refunds** are calculated at 80% of the booking total
- The **database** (`ticketsystem.db`) is created automatically on first run
- Default tickets (Cab, Plane, Train) are seeded if the database is empty
