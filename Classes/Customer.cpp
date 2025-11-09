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

void Customer::setAge(int age) {
	this->age = age;
}

Customer::Customer(const Customer& other)
	: User(other), age(other.age)
{
}

Customer::~Customer() {
	// Destructor
}