#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include "Admin.h"
#include "Customer.h"

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

int main() {
    cout << "========================================" << endl;
    cout << "   Welcome to Ticket Booking System    " << endl;
    cout << "========================================" << endl;
    cout << endl;

    handleMainMenu();

    cout << "\nThank you for using the Ticket Booking System!" << endl;
    return 0;
}

void displayMainMenu() {
    cout << "\n--- Main Menu ---" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Browse Available Tickets (Guest)" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayCustomerMenu() {
    cout << "\n--- Customer Menu ---" << endl;
    if (currentUser) {
        cout << "Welcome, " << currentUser->getFullName() << "!" << endl;
    }
    cout << "1. Book Ticket" << endl;
    cout << "2. View My Tickets" << endl;
    cout << "3. Cancel Ticket" << endl;
    cout << "4. Browse Available Tickets" << endl;
    cout << "5. Logout" << endl;
    cout << "Enter your choice: ";
}

void displayAdminMenu() {
    cout << "\n--- Admin Menu ---" << endl;
    if (currentUser) {
        cout << "Welcome, Admin " << currentUser->getFullName() << "!" << endl;
    }
    cout << "1. View All Tickets" << endl;
    cout << "2. Manage Bookings (Add/Edit/Delete)" << endl;
    cout << "3. View Customer List" << endl;
    cout << "4. Generate Reports" << endl;
    cout << "5. Logout" << endl;
    cout << "Enter your choice: ";
}

void handleMainMenu() {
    int choice;
    bool exit = false;

    while (!exit) {
        displayMainMenu();
        cin >> choice;

        // Input validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            pauseScreen();
            continue;
        }

        clearScreen();

        switch (choice) {
            case 1:
                login();
                if (isLoggedIn && currentUser) {
                    // Try to cast to Admin first
                    shared_ptr<Admin> admin = dynamic_pointer_cast<Admin>(currentUser);
                    if (admin) {
                        handleAdminMenu(admin);
                    } else {
                        // Must be a Customer
                        shared_ptr<Customer> customer = dynamic_pointer_cast<Customer>(currentUser);
                        if (customer) {
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
                cout << "Available Transportation Tickets:" << endl;
                cout << "1. Cab - City Taxi Service (Available: Now, Price: $25)" << endl;
                cout << "2. Plane - Flight to New York (Date: 2025-12-01, Price: $450)" << endl;
                cout << "3. Train - Express to Boston (Date: 2025-11-25, Price: $85)" << endl;
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

void handleCustomerMenu(shared_ptr<Customer> customer) {
    int choice;
    bool logout = false;

    while (!logout && isLoggedIn) {
        displayCustomerMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            pauseScreen();
            continue;
        }

        clearScreen();

        switch (choice) {
            case 1: {
                customer->browseEvents();
                int ticketChoice, numTickets;
                while (true) {
                    cout << "\nSelect ticket type (1-3): ";
                    cin >> ticketChoice;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Please enter a number for ticket type." << endl;
                        continue;
                    }
                    cout << "Number of tickets: ";
                    cin >> numTickets;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Please enter a number for number of tickets." << endl;
                        continue;
                    }
                    if (ticketChoice >= 1 && ticketChoice <= 3 && numTickets > 0) {
                        break;
                    } else {
                        cout << "\nInvalid selection! Please try again." << endl;
                    }
                }

                string bookingId = "BK" + to_string(rand() % 10000);
                customer->bookTicket(bookingId);
                cout << "Confirmation will be sent to " << customer->getEmail() << endl;
                pauseScreen();
                break;
            }
            case 2:
                customer->viewMyTickets();
                pauseScreen();
                break;
            case 3: {
                string bookingId;
                cout << "Enter Booking ID to cancel: ";
                cin >> bookingId;
                customer->cancelTicket(bookingId);
                pauseScreen();
                break;
            }
            case 4:
                customer->browseEvents();
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

void handleAdminMenu(shared_ptr<Admin> admin) {
    int choice;
    bool logout = false;

    while (!logout && isLoggedIn) {
        displayAdminMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            pauseScreen();
            continue;
        }

        clearScreen();

        switch (choice) {
            case 1:
                admin->viewAllTickets();
                pauseScreen();
                break;
            case 2:
                admin->manageBookings();
                pauseScreen();
                break;
            case 3:
                admin->viewCustomerList();
                pauseScreen();
                break;
            case 4:
                admin->generateReports();
                pauseScreen();
                break;
            case 5:
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

void login() {
    cout << "--- Login ---" << endl;
    string username, password;
    
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    // TODO: Implement actual authentication with database
    // For now, simple demo logic
    if (username == "admin" && password == "admin") {
        // Create Admin user object
        currentUser = make_shared<Admin>(1, username, password, "Administrator", "admin@ticketsystem.com", "super");
        if (currentUser->login(username, password)) {
            isLoggedIn = true;
        }
    } else if (!username.empty() && !password.empty()) {
        // Create Customer user object
        currentUser = make_shared<Customer>(2, username, password, username, username + "@example.com", "555-0000");
        if (currentUser->login(username, password)) {
            isLoggedIn = true;
        }
    } else {
        cout << "\nLogin failed! Invalid credentials." << endl;
    }
    
    pauseScreen();
    clearScreen();
}

void registerUser() {
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
    cin >> email;
    cout << "Phone: ";
    cin >> phone;

    // TODO: Implement actual registration logic with database
    // For now, just create a Customer object to validate the input
    int newId = rand() % 1000 + 100; // Generate random ID for demo
    Customer newCustomer(newId, username, password, fullName, email, phone);
    
    cout << "\nRegistration successful! You can now login." << endl;
    cout << "Welcome, " << newCustomer.getFullName() << "!" << endl;
    pauseScreen();
}

void clearScreen() {
    // Use ANSI escape codes to clear the screen and move cursor to home position.
    // This works on most Unix-like systems and modern Windows terminals.
    cout << "\033[2J\033[H" << flush;
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
