#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Function prototypes
void displayMainMenu();
void displayCustomerMenu();
void displayAdminMenu();
void handleMainMenu();
void handleCustomerMenu();
void handleAdminMenu();
void login();
void registerUser();
void bookTicket();
void viewMyTickets();
void cancelTicket();
void viewAllTickets();
void manageEvents();
void clearScreen();
void pauseScreen();

// Global variable to track login status
bool isLoggedIn = false;
bool isAdmin = false;
string currentUser = "";

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
    cout << "3. Browse Events (Guest)" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

void displayCustomerMenu() {
    cout << "\n--- Customer Menu ---" << endl;
    cout << "Welcome, " << currentUser << "!" << endl;
    cout << "1. Book Ticket" << endl;
    cout << "2. View My Tickets" << endl;
    cout << "3. Cancel Ticket" << endl;
    cout << "4. Browse Events" << endl;
    cout << "5. Logout" << endl;
    cout << "Enter your choice: ";
}

void displayAdminMenu() {
    cout << "\n--- Admin Menu ---" << endl;
    cout << "Welcome, Admin " << currentUser << "!" << endl;
    cout << "1. View All Tickets" << endl;
    cout << "2. Manage Events (Add/Edit/Delete)" << endl;
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
                if (isLoggedIn) {
                    if (isAdmin) {
                        handleAdminMenu();
                    } else {
                        handleCustomerMenu();
                    }
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                cout << "--- Browse Events (Guest Mode) ---" << endl;
                cout << "Event listing feature coming soon..." << endl;
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

void handleCustomerMenu() {
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
            case 1:
                bookTicket();
                break;
            case 2:
                viewMyTickets();
                break;
            case 3:
                cancelTicket();
                break;
            case 4:
                cout << "--- Browse Events ---" << endl;
                cout << "Event listing feature coming soon..." << endl;
                pauseScreen();
                break;
            case 5:
                cout << "Logging out..." << endl;
                isLoggedIn = false;
                isAdmin = false;
                currentUser = "";
                logout = true;
                pauseScreen();
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                pauseScreen();
        }
    }
}

void handleAdminMenu() {
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
                viewAllTickets();
                break;
            case 2:
                manageEvents();
                break;
            case 3:
                cout << "--- Customer List ---" << endl;
                cout << "Customer management feature coming soon..." << endl;
                pauseScreen();
                break;
            case 4:
                cout << "--- Generate Reports ---" << endl;
                cout << "Reporting feature coming soon..." << endl;
                pauseScreen();
                break;
            case 5:
                cout << "Logging out..." << endl;
                isLoggedIn = false;
                isAdmin = false;
                currentUser = "";
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

    // TODO: Implement authentication logic
    if (username == "admin" && password == "admin") {
        isLoggedIn = true;
        isAdmin = true;
        currentUser = username;
        cout << "\nLogin successful! Welcome Admin." << endl;
    } else if (!username.empty() && !password.empty()) {
        isLoggedIn = true;
        isAdmin = false;
        currentUser = username;
        cout << "\nLogin successful! Welcome " << username << "." << endl;
    } else {
        cout << "\nLogin failed! Invalid credentials." << endl;
    }
    
    pauseScreen();
    clearScreen();
}

void registerUser() {
    cout << "--- Register New User ---" << endl;
    string username, password, email, phone;
    
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    cout << "Email: ";
    cin >> email;
    cout << "Phone: ";
    cin >> phone;

    // TODO: Implement registration logic
    cout << "\nRegistration successful! You can now login." << endl;
    pauseScreen();
}

void bookTicket() {
    cout << "--- Book Ticket ---" << endl;
    cout << "Available Events:" << endl;
    cout << "1. Concert - Rock Band Live (Date: 2025-12-01, Price: $50)" << endl;
    cout << "2. Movie - Action Thriller (Date: 2025-11-20, Price: $15)" << endl;
    cout << "3. Sports - Basketball Game (Date: 2025-11-25, Price: $30)" << endl;
    
    int eventChoice, numTickets;
    cout << "\nSelect event (1-3): ";
    cin >> eventChoice;
    cout << "Number of tickets: ";
    cin >> numTickets;

    // TODO: Implement actual booking logic
    if (eventChoice >= 1 && eventChoice <= 3 && numTickets > 0) {
        cout << "\nBooking successful! Confirmation will be sent to your email." << endl;
        cout << "Booking ID: BK" << rand() % 10000 << endl;
    } else {
        cout << "\nInvalid selection!" << endl;
    }
    
    pauseScreen();
}

void viewMyTickets() {
    cout << "--- My Tickets ---" << endl;
    // TODO: Implement ticket retrieval
    cout << "Your booked tickets:" << endl;
    cout << "1. Booking ID: BK1234 | Event: Concert - Rock Band Live | Tickets: 2 | Date: 2025-12-01" << endl;
    cout << "2. Booking ID: BK5678 | Event: Movie - Action Thriller | Tickets: 1 | Date: 2025-11-20" << endl;
    pauseScreen();
}

void cancelTicket() {
    cout << "--- Cancel Ticket ---" << endl;
    string bookingId;
    cout << "Enter Booking ID to cancel: ";
    cin >> bookingId;

    // TODO: Implement cancellation logic
    cout << "\nTicket cancellation request received for Booking ID: " << bookingId << endl;
    cout << "Refund will be processed within 7 business days." << endl;
    pauseScreen();
}

void viewAllTickets() {
    cout << "--- All Tickets (Admin View) ---" << endl;
    // TODO: Implement admin ticket view
    cout << "All system bookings:" << endl;
    cout << "1. User: john_doe | Booking ID: BK1234 | Event: Concert | Tickets: 2" << endl;
    cout << "2. User: jane_smith | Booking ID: BK5678 | Event: Movie | Tickets: 1" << endl;
    cout << "3. User: mike_jones | Booking ID: BK9012 | Event: Sports | Tickets: 4" << endl;
    pauseScreen();
}

void manageEvents() {
    cout << "--- Manage Events ---" << endl;
    cout << "1. Add New Event" << endl;
    cout << "2. Edit Event" << endl;
    cout << "3. Delete Event" << endl;
    cout << "4. Back" << endl;
    
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    // TODO: Implement event management logic
    switch (choice) {
        case 1:
            cout << "\nAdd event feature coming soon..." << endl;
            break;
        case 2:
            cout << "\nEdit event feature coming soon..." << endl;
            break;
        case 3:
            cout << "\nDelete event feature coming soon..." << endl;
            break;
        case 4:
            return;
        default:
            cout << "\nInvalid choice!" << endl;
    }
    
    pauseScreen();
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
