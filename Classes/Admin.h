#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include "User.h" // Include the base User class
#endif // !ADMIN_H

class Admin : public User {
	public:

	Admin(); // default constructor

	Admin(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&); // parameterized constructor

	Admin(const Admin&); // copy constructor

	void setAdminCode(const std::string&); // set admin code

	std::string getAdminCode() const; // return admin code

	void addAdminToDatabase();

	~Admin(); // destructor

	private:
		std::string adminCode;
};