#include <iostream>
#include "Customer.h"

using namespace std;

Customer::Customer()
{
	age = 0;
}

Customer::Customer(const string& uname, const string& pwd, const string& fname, const string& eml, int age)
	: User(uname, pwd, fname, eml)
{
	setAge(age);
}

Customer::Customer(const Customer& other)
	: User(other), age(other.age)
{
}

void Customer::setAge(int age) {
	this->age = age;
}

bool Customer::purchaseTicket() 
{
	// Implementation for purchasing a ticket goes here
	return true; // Placeholder return value
}

string Customer::viewTickets() 
{
	// Implementation for viewing tickets goes here
	return ""; // Placeholder return value
}

bool Customer::cancelTicket() 
{
	// Implementation for canceling a ticket goes here
	return true; // Placeholder return value
}

Customer::~Customer() {
	// Destructor
}