#include <iostream>
#include "Admin.h"

using namespace std;

Admin::Admin()
{
	adminCode = "";
}

Admin::Admin(const string& uname, const string& pwd, const string& fname, const string& eml, const string& code)
	: User(uname, pwd, fname, eml), adminCode(code)
{
	setAdminCode(code);
}

void Admin::setAdminCode(const string& code) 
{
	adminCode = code;
}

bool Admin::createTicket() 
{
	// Implementation for creating a ticket goes here
	return true; // Placeholder return value
}

Admin::~Admin()
{
	// Destructor
}