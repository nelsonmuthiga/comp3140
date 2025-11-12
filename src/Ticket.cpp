#include <iostream>
#include "Ticket.h"

using namespace std;

// Ticket class member function definitions

// Default constructor
Ticket::Ticket() : ticketId(""), status(""), user(), date(), price(0.0), vehicle(), origin(""), destination("") {}

// Parameterized constructor
Ticket::Ticket(const string& ticketId, const string& status, const BaseUser user, const date::date& date, float price, const Vehicle vehicle, const string& origin, const string& destination)
	: ticketId(ticketId), status(status), user(user), date(date), price(price), vehicle(vehicle), origin(origin), destination(destination) {}

// Setter methods
void Ticket::setTicketId(const string& ticketId) {
	this->ticketId = ticketId;
}

void Ticket::setStatus(const string& status) {
	this->status = status;
}

void Ticket::setUser(const BaseUser user) {
	this->user = user;
}

void Ticket::setDate(const date::date& date) {
	this->date = date;
}

void Ticket::setPrice(float price) {
	this->price = price;
}

void Ticket::setVehicle(const Vehicle vehicle) {
	this->vehicle = vehicle;
}

void Ticket::setOrigin(const string& origin) {
	this->origin = origin;
}

void Ticket::setDestination(const string& destination) {
	this->destination = destination;
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

date::date Ticket::getDate() const {
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