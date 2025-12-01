#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include "Admin.h"
#include "Customer.h"
#include "BookingService.h"
#include "TicketService.h"
#include "AdminService.h"
#include "VehicleService.h"
#include "Database.h"

using namespace std;

// Function prototypes
void displayMainMenu();
void displayCustomerMenu();
void displayAdminMenu();
void handleMainMenu();
void handleCustomerMenu(shared_ptr<Customer> customer);
void handleAdminMenu(shared_ptr<Admin> admin);
void login();
void registerUser();
void clearScreen();
void pauseScreen();

// Global variable to track login status
shared_ptr<BaseUser> currentUser = nullptr;
bool isLoggedIn = false;

// Global service instances
BookingService bookingService;
TicketService ticketService;
AdminService adminService;
VehicleService vehicleService;

int main()
{
    // Initialize database connection
    if (!Database::getInstance().initialize("ticketsystem.db"))
    {
        cerr << "Failed to initialize database. Exiting." << endl;
        return 1;
    }

    cout << "========================================" << endl;
    cout << "   Welcome to Ticket Booking System    " << endl;
    cout << "========================================" << endl;
    cout << endl;

    handleMainMenu();

    cout << "\nThank you for using the Ticket Booking System!" << endl;
    return 0;
}

void displayMainMenu()
{
    cout << "\n--- Main Menu ---" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Browse Available Tickets (Guest)" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayCustomerMenu()
{
    cout << "\n--- Customer Menu ---" << endl;
    if (currentUser)
    {
        cout << "Welcome, " << currentUser->getFullName() << "!" << endl;
    }
    cout << "1. Book Ticket" << endl;
    cout << "2. View My Tickets" << endl;
    cout << "3. Cancel Ticket" << endl;
    cout << "4. Browse Available Tickets" << endl;
    cout << "5. Logout" << endl;
    cout << "Enter your choice: ";
}

void displayAdminMenu()
{
    cout << "\n--- Admin Menu ---" << endl;
    if (currentUser)
    {
        cout << "Welcome, Admin " << currentUser->getFullName() << "!" << endl;
    }
    cout << "1. View All Tickets" << endl;
    cout << "2. Manage Tickets (Add/Edit/Delete)" << endl;
    cout << "3. View Customer List" << endl;
    cout << "4. View Vehicles (OOP)" << endl;
    cout << "5. Manage Vehicles (Add Vehicle)" << endl;
    cout << "6. Generate Reports" << endl;
    cout << "7. Logout" << endl;
    cout << "Enter your choice: ";
}

void handleMainMenu()
{
    int choice;
    bool exit = false;

    while (!exit)
    {
        displayMainMenu();
        cin >> choice;

        // Input validation
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            pauseScreen();
            continue;
        }

        clearScreen();

        switch (choice)
        {
        case 1:
            login();
            if (isLoggedIn && currentUser)
            {
                // Try to cast to Admin first
                shared_ptr<Admin> admin = dynamic_pointer_cast<Admin>(currentUser);
                if (admin)
                {
                    handleAdminMenu(admin);
                }
                else
                {
                    // Must be a Customer
                    shared_ptr<Customer> customer = dynamic_pointer_cast<Customer>(currentUser);
                    if (customer)
                    {
                        handleCustomerMenu(customer);
                    }
                }
            }
            break;
        case 2:
            registerUser();
            break;
        case 3:
            cout << "--- Browse Available Tickets (Guest Mode) ---" << endl;
            ticketService.displayAvailableTickets();
            pauseScreen();
            break;
        case 4:
            exit = true;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
        }
    }
}

void handleCustomerMenu(shared_ptr<Customer> customer)
{
    int choice;
    bool logout = false;

    while (!logout && isLoggedIn)
    {
        displayCustomerMenu();
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            pauseScreen();
            continue;
        }

        clearScreen();

        switch (choice)
        {
        case 1:
        {
            // Use TicketService to display available tickets
            auto tickets = ticketService.getAvailableTickets();
            cout << "--- Available Tickets ---" << endl;
            for (size_t i = 0; i < tickets.size(); ++i)
            {
                cout << (i + 1) << ". " << tickets[i].type
                     << " from " << tickets[i].origin << " to " << tickets[i].destination
                     << " (Date: " << tickets[i].date
                     << ", Price: $" << tickets[i].price << ")" << endl;
            }

            int ticketChoice, numTickets;
            while (true)
            {
                cout << "\nSelect ticket type (1-" << tickets.size() << "): ";
                cin >> ticketChoice;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number for ticket type." << endl;
                    continue;
                }
                cout << "Number of tickets (how many seats to book): ";
                cin >> numTickets;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a number for number of tickets." << endl;
                    continue;
                }
                if (ticketChoice >= 1 && ticketChoice <= static_cast<int>(tickets.size()) && numTickets > 0)
                {
                    break;
                }
                else
                {
                    cout << "\nInvalid selection! Please try again." << endl;
                }
            }

            // Use BookingService to create booking
            string bookingId = bookingService.createBooking(
                customer,
                tickets[ticketChoice - 1].id,
                numTickets);

            if (!bookingId.empty())
            {
                customer->addBookingId(bookingId);
                cout << "Confirmation will be sent to " << customer->getEmail() << endl;
            }
            else
            {
                cout << "Booking failed. Please try again." << endl;
            }
            pauseScreen();
            break;
        }
        case 2:
        {
            // Use BookingService to get customer bookings
            auto bookings = bookingService.getCustomerBookings(customer->getId());
            cout << "--- My Tickets ---" << endl;
            if (bookings.empty())
            {
                cout << "You have no tickets booked yet." << endl;
            }
            else
            {
                cout << "Your booked tickets:" << endl;
                for (const auto &booking : bookings)
                {
                    cout << booking << endl;
                }
            }
            pauseScreen();
            break;
        }
        case 3:
        {
            string bookingId;
            cout << "Enter Booking ID to cancel: ";
            cin >> bookingId;

            // Use BookingService to cancel booking
            if (bookingService.cancelBooking(bookingId, customer))
            {
                customer->removeBookingId(bookingId);
                cout << "Cancellation successful!" << endl;
            }
            else
            {
                cout << "Cancellation failed." << endl;
            }
            pauseScreen();
            break;
        }
        case 4:
            // Use TicketService to browse tickets
            ticketService.displayAvailableTickets();
            pauseScreen();
            break;
        case 5:
            cout << "Logging out..." << endl;
            customer->logout();
            isLoggedIn = false;
            currentUser = nullptr;
            logout = true;
            pauseScreen();
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
        }
    }
}

void handleAdminMenu(shared_ptr<Admin> admin)
{
    int choice;
    bool logout = false;

    while (!logout && isLoggedIn)
    {
        displayAdminMenu();
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            pauseScreen();
            continue;
        }

        clearScreen();

        switch (choice)
        {
        case 1:
        {
            // Display ticket inventory
            cout << "--- All Tickets (Inventory) ---" << endl;
            ticketService.displayAvailableTickets();
            pauseScreen();
            break;
        }
        case 2:
        {
            // Use TicketService for ticket management
            cout << "--- Manage Tickets ---" << endl;
            cout << "1. Add New Ticket" << endl;
            cout << "2. Edit Ticket" << endl;
            cout << "3. Delete Ticket" << endl;
            cout << "4. Back" << endl;

            int manageChoice;
            cout << "Enter your choice: ";
            cin >> manageChoice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
            }
            else
            {
                clearScreen();
                switch (manageChoice)
                {
                case 1:
                {
                    cout << "\n=== Create New Ticket ===" << endl;

                    // Step 1: Show all vehicles
                    auto vehicles = Database::getInstance().getAllVehicles();
                    if (vehicles.empty())
                    {
                        cout << "No vehicles available. Please add vehicles first." << endl;
                        break;
                    }

                    cout << "\nAvailable Vehicles:" << endl;
                    for (size_t i = 0; i < vehicles.size(); ++i)
                    {
                        cout << (i + 1) << ". " << vehicles[i].vehicleType
                             << " (ID: " << vehicles[i].id << ", Capacity: " << vehicles[i].capacity << ")";

                        // Show vehicle-specific details
                        if (vehicles[i].vehicleType == "Plane")
                            cout << " - Airline: " << vehicles[i].airline;
                        else if (vehicles[i].vehicleType == "Cab")
                            cout << " - License: " << vehicles[i].licensePlate;
                        else if (vehicles[i].vehicleType == "Train")
                            cout << " - Train: " << vehicles[i].trainNumber;
                        cout << endl;
                    }

                    // Step 2: Select vehicle
                    int vehicleChoice;
                    cout << "\nSelect vehicle (1-" << vehicles.size() << "): ";
                    cin >> vehicleChoice;

                    if (vehicleChoice < 1 || vehicleChoice > static_cast<int>(vehicles.size()))
                    {
                        cout << "Invalid choice!" << endl;
                        break;
                    }

                    auto selectedVehicle = vehicles[vehicleChoice - 1];

                    // Step 3: Create ticket using selected vehicle info
                    TicketInfo newTicket;
                    newTicket.id = 0;
                    newTicket.type = selectedVehicle.vehicleType; // Auto-set from vehicle
                    newTicket.vehicleId = selectedVehicle.id;     // Link to vehicle

                    cout << "\nCreating " << selectedVehicle.vehicleType << " ticket..." << endl;
                    cout << "Enter origin (starting location): ";
                    cin.ignore();
                    getline(cin, newTicket.origin);

                    cout << "Enter destination (ending location): ";
                    getline(cin, newTicket.destination);

                    cout << "Enter price: $";
                    cin >> newTicket.price;

                    cout << "Enter availability (max " << selectedVehicle.capacity << " seats): ";
                    cin >> newTicket.availability;

                    cout << "Enter date (YYYY-MM-DD): ";
                    cin.ignore();
                    getline(cin, newTicket.date);

                    if (ticketService.createTicket(newTicket))
                    {
                        cout << "\nTicket created successfully!" << endl;
                    }
                    else
                    {
                        cout << "\nFailed to create ticket." << endl;
                    }
                    break;
                }
                case 2:
                {
                    // Show available tickets first
                    cout << "--- Current Tickets ---" << endl;
                    ticketService.displayAvailableTickets();
                    cout << endl;

                    int ticketId;
                    cout << "Enter ticket ID to edit: ";
                    cin >> ticketId;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Error: Invalid ticket ID." << endl;
                        break;
                    }

                    TicketInfo existingTicket = ticketService.getTicketById(ticketId);
                    if (existingTicket.id == 0)
                    {
                        cout << "Error: Ticket not found." << endl;
                        break;
                    }

                    cout << "\nEditing: " << existingTicket.type << " - From: " << existingTicket.origin << " To: " << existingTicket.destination << endl;
                    cout << "Current price: $" << existingTicket.price << " | Availability: " << existingTicket.availability << endl;

                    TicketInfo updatedTicket;
                    updatedTicket.id = ticketId;
                    updatedTicket.vehicleId = existingTicket.vehicleId; // Preserve vehicle link

                    cout << "\nEnter new type (current: " << existingTicket.type << ", case insensitive): ";
                    cin >> updatedTicket.type;

                    cout << "Enter new origin: ";
                    cin.ignore();
                    getline(cin, updatedTicket.origin);

                    cout << "Enter new destination: ";
                    getline(cin, updatedTicket.destination);

                    cout << "Enter new price (e.g., 150.50): $";
                    cin >> updatedTicket.price;

                    cout << "Enter new availability (number of seats): ";
                    cin >> updatedTicket.availability;

                    cout << "Enter new date (format: YYYY-MM-DD, e.g., 2025-12-25): ";
                    cin.ignore();
                    getline(cin, updatedTicket.date);

                    ticketService.modifyTicket(ticketId, updatedTicket);
                    break;
                }
                case 3:
                {
                    // Show available tickets first
                    cout << "--- Current Tickets ---" << endl;
                    ticketService.displayAvailableTickets();
                    cout << endl;

                    int ticketId;
                    cout << "Enter ticket ID to delete: ";
                    cin >> ticketId;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Error: Invalid ticket ID." << endl;
                        break;
                    }

                    TicketInfo existingTicket = ticketService.getTicketById(ticketId);
                    if (existingTicket.id == 0)
                    {
                        cout << "Error: Ticket not found." << endl;
                        break;
                    }

                    cout << "Are you sure you want to delete '" << existingTicket.type
                         << " from " << existingTicket.origin << " to " << existingTicket.destination << "'? (y/n): ";
                    char confirm;
                    cin >> confirm;

                    if (confirm == 'y' || confirm == 'Y')
                    {
                        ticketService.deleteTicket(ticketId);
                    }
                    else
                    {
                        cout << "Deletion cancelled." << endl;
                    }
                    break;
                }
                }
            }
            pauseScreen();
            break;
        }
        case 3:
            // Use AdminService to view customers
            adminService.displayCustomerList();
            pauseScreen();
            break;
        case 4:
            // Use VehicleService to view vehicles
            vehicleService.displayAllVehicles();
            pauseScreen();
            break;
        case 5:
        {
            // Vehicle Management (Add Vehicle)
            cout << "--- Add New Vehicle ---" << endl;
            cout << "1. Add Plane" << endl;
            cout << "2. Add Cab" << endl;
            cout << "3. Add Train" << endl;
            cout << "4. Back" << endl;

            int vehicleChoice;
            cout << "Enter your choice: ";
            cin >> vehicleChoice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
            }
            else
            {
                clearScreen();
                switch (vehicleChoice)
                {
                case 1:
                {
                    // Add Plane
                    string airline, flightNumber, flightNo;
                    int capacity;

                    cout << "\n=== Add New Plane ===" << endl;
                    cout << "Enter airline name (e.g., AirExpress): ";
                    cin.ignore();
                    getline(cin, airline);
                    cout << "Enter flight number (e.g., AE123): ";
                    getline(cin, flightNumber);
                    cout << "Enter flight no (e.g., AE123): ";
                    getline(cin, flightNo);
                    cout << "Enter capacity (number of seats, e.g., 180): ";
                    cin >> capacity;

                    int vehicleId = vehicleService.createPlane(airline, flightNumber, flightNo, capacity);
                    if (vehicleId > 0)
                    {
                        cout << "\n✓ Plane created successfully with ID: " << vehicleId << endl;
                    }
                    else
                    {
                        cout << "\n✗ Failed to create plane." << endl;
                    }
                    break;
                }
                case 2:
                {
                    // Add Cab
                    string licensePlate, driverName;
                    int capacity;

                    cout << "\n=== Add New Cab ===" << endl;
                    cout << "Enter license plate (e.g., ABC123): ";
                    cin.ignore();
                    getline(cin, licensePlate);
                    cout << "Enter driver name (e.g., John Smith): ";
                    getline(cin, driverName);
                    cout << "Enter capacity (number of seats, e.g., 4): ";
                    cin >> capacity;

                    int vehicleId = vehicleService.createCab(licensePlate, driverName, capacity);
                    if (vehicleId > 0)
                    {
                        cout << "\n✓ Cab created successfully with ID: " << vehicleId << endl;
                    }
                    else
                    {
                        cout << "\n✗ Failed to create cab." << endl;
                    }
                    break;
                }
                case 3:
                {
                    // Add Train
                    string trainNumber, platform;
                    int capacity;

                    cout << "\n=== Add New Train ===" << endl;
                    cout << "Enter train number (e.g., TR100): ";
                    cin.ignore();
                    getline(cin, trainNumber);
                    cout << "Enter platform (e.g., Platform 1): ";
                    getline(cin, platform);
                    cout << "Enter capacity (number of seats, e.g., 500): ";
                    cin >> capacity;

                    int vehicleId = vehicleService.createTrain(trainNumber, platform, capacity);
                    if (vehicleId > 0)
                    {
                        cout << "\n✓ Train created successfully with ID: " << vehicleId << endl;
                    }
                    else
                    {
                        cout << "\n✗ Failed to create train." << endl;
                    }
                    break;
                }
                }
            }
            pauseScreen();
            break;
        }
        case 6:
        {
            // Use AdminService for reports
            adminService.displayReportsMenu();
            int reportChoice;
            cout << "Enter your choice: ";
            cin >> reportChoice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input!" << endl;
            }
            else
            {
                clearScreen();
                switch (reportChoice)
                {
                case 1:
                    cout << adminService.generateSalesReport();
                    break;
                case 2:
                    cout << adminService.generateCustomerReport();
                    break;
                case 3:
                    cout << adminService.generateBookingStatistics();
                    break;
                }
            }
            pauseScreen();
            break;
        }
        case 7:
            cout << "Logging out..." << endl;
            admin->logout();
            isLoggedIn = false;
            currentUser = nullptr;
            logout = true;
            pauseScreen();
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pauseScreen();
        }
    }
}

void login()
{
    cout << "--- Login ---" << endl;
    string username, password;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    // Authenticate using database
    UserRecord user = Database::getInstance().getUserByUsername(username);

    if (user.id != 0 && user.password == password)
    {
        if (user.userType == "admin")
        {
            currentUser = make_shared<Admin>(user.id, user.username, user.password, user.fullName, user.email);
        }
        else
        {
            currentUser = make_shared<Customer>(user.id, user.username, user.password, user.fullName, user.email, user.phone);
        }

        if (currentUser->login(username, password))
        {
            isLoggedIn = true;
            cout << "\nLogin successful! Welcome, " << user.fullName << "!" << endl;
        }
    }
    else
    {
        cout << "\nLogin failed! Invalid credentials." << endl;
    }

    pauseScreen();
    clearScreen();
}

void registerUser()
{
    cout << "--- Register New User ---" << endl;
    string username, password, fullName, email, phone;

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    cout << "Full Name: ";
    cin.ignore();
    getline(cin, fullName);
    cout << "Email: ";
    getline(cin, email);
    cout << "Phone (optional, press Enter to skip): ";
    getline(cin, phone);

    if(username.empty() || password.empty() || fullName.empty() || email.empty())
    {
        cout << "\nRegistration failed! All fields except phone are required." << endl;
        pauseScreen();
        return;
    }
    
    // Check if username already exists
    if (Database::getInstance().userExists(username))
    {
        cout << "\nRegistration failed! Username already exists." << endl;
        pauseScreen();
        return;
    }

    // Create user in database
    if (Database::getInstance().createUser(username, password, fullName, email, phone, "customer"))
    {
        cout << "\nRegistration successful! You can now login." << endl;
        cout << "Welcome, " << fullName << "!" << endl;
    }
    else
    {
        cout << "\nRegistration failed! Please try again." << endl;
    }
    pauseScreen();
}

void clearScreen()
{
    // Use ANSI escape codes to clear the screen and move cursor to home position.
    cout << "\033[2J\033[H" << flush;
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
