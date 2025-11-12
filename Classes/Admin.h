#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include "User.h" // Include the base User class


class Admin : public User {
	public:

	Admin(); // default constructor

	Admin(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&); // parameterized constructor

	void setAdminCode(const std::string&); // set admin code

	bool createTicket(); // create a ticket

	~Admin(); // destructor

	private:
		std::string adminCode;
};
#endif // !ADMIN_H