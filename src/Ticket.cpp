#include <iostream>
#include "Ticket.h"

using namespace std;

// Default constructor
Ticket::Ticket() 
    : ticketId(""), status(""), user(), date(), price(0.0), 
      vehicle(), origin(""), destination("") {}

// Parameterized constructor - FIXED
Ticket::Ticket(const string& tId, const string& stat, const BaseUser& usr, 
               const chrono::system_clock::time_point& dt, float pr, 
               const Vehicle& veh, const string& orig, const string& dest)
    : ticketId(tId), status(stat), user(usr), date(dt), price(pr), 
      vehicle(veh), origin(orig), destination(dest) {}

// Setter methods
void Ticket::setTicketId(const string& tId) {
    this->ticketId = tId;
}

void Ticket::setStatus(const string& stat) {
    this->status = stat;
}

void Ticket::setUser(const BaseUser& usr) {
    this->user = usr;
}

void Ticket::setDate(const chrono::system_clock::time_point& dt) {
    this->date = dt;
}

void Ticket::setPrice(float pr) {
    this->price = pr;
}

void Ticket::setVehicle(const Vehicle& veh) {
    this->vehicle = veh;
}

void Ticket::setOrigin(const string& orig) {
    this->origin = orig;
}

void Ticket::setDestination(const string& dest) {
    this->destination = dest;
}

// Getter methods
string Ticket::getTicketId() const {
    return ticketId;
}

string Ticket::getStatus() const {
    return status;
}

BaseUser Ticket::getUser() const {
    return user;
}

chrono::system_clock::time_point Ticket::getDate() const {
    return date;
}

float Ticket::getPrice() const {
    return price;
}

Vehicle Ticket::getVehicle() const {
    return vehicle;
}

string Ticket::getOrigin() const {
    return origin;
}

string Ticket::getDestination() const {
    return destination;
}

Ticket::~Ticket() {
    // Destructor
}