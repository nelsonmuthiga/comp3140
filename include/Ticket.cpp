#include <iostream>
#include "Ticket.h"

using namespace std;

Ticket::Ticket() : eventName(""), date(""), price(0.0) {}

Ticket::Ticket(const string& evtName, const string& dt, double prc)
	: eventName(evtName), date(dt), price(prc)
{
}

void Ticket::setEventName(const string& evtName) {
	eventName = evtName;
}

void Ticket::setDate(const string& dt) {
	date = dt;
}

void Ticket::setPrice(double prc) {
	price = prc;
}

string Ticket::getEventName() const {
	return eventName;
}

string Ticket::getDate() const {
	return date;
}

double Ticket::getPrice() const {
	return price;
}

Ticket::~Ticket() {
	// Destructor
}