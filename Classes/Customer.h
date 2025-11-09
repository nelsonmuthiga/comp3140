#ifndef CUSTOMER_USER_H
#define CUSTOMER_USER_H
#include <string>
#include "User.h" // Include the base User class
#endif // !CUSTOMER_USER_H

class Customer : public User {
	public:

	Customer(); // default constructor

	Customer(const std::string&, const std::string&, const std::string&, const std::string&, int = 0); // parameterized constructor

	Customer(const Customer&); // copy constructor

	void setAge(int); // set age

	void addCustomerToDatabase();

	~Customer(); // destructor
	private:
		int age;
};