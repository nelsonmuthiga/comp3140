#ifndef TICKET_H
#define TICKET_H
#include <string>

class Ticket {

	public:

	Ticket(); // default constructor

	Ticket(const std::string&, const std::string&, double); // parameterized constructor

	void setEventName(const std::string&); // set event name

	void setDate(const std::string&); // set date

	void setPrice(double); // set price

	std::string getEventName() const; // return event name

	std::string getDate() const; // return date

	double getPrice() const; // return price

	~Ticket(); // destructor

	private:

		std::string eventName;
		std::string date;
		double price;
};
#endif
