#include <iostream>
#include "Ticket.h"

using namespace std;

Ticket::Ticket() : id(0), companyName(""), date(""), price(0.0) {}

Ticket::Ticket(int no, const string& cmpName, const string& dt, double prc)
	: id(no), companyName(cmpName), date(dt), price(prc)
{
}

void Ticket::setCompanyName(const string& cmpName) {
	this->companyName = cmpName;
}

void Ticket::setDate(const string& dt) {
	this->date = dt;
}

void Ticket::setPrice(double prc) {
	this->price = prc;
}

string Ticket::getCompanyName() const {
	return companyName;
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