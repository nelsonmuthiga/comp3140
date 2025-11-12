#ifndef TICKET_H
#define TICKET_H
#include <string>
#include "BaseUser.h"
#include <date/date.h>
#include 'Vehicle.h'

class Ticket {

	public:

	Ticket(); // default constructor

	Ticket(const std::string&, const std::string&, const BaseUser user&, const date::date&, float, const Vehicle vehicle, const std::string&, const std::string&); // parameterized constructor

	void setTicketId(const std::string&); // set event name

	void setStatus(const std::string&); // set status

	void setUser(const BaseUser user); // set user

	void setDate(const date::date&); // set date

	void setPrice(float); // set price

	void setVehicle(const Vehicle vehicle); // set vehicle

	void setOrigin(const std::string&); // set origin

	void setDestination(const std::string&); // set destination

	std::string getTicketId() const; // return ticket id

	std::string getStatus() const; // return status

	BaseUser getUser() const; // return user

	date::date getDate() const; // return date

	float getPrice() const; // return price

	Vehicle getVehicle() const; // return vehicle

	std::string getOrigin() const; // return origin

	std::string getDestination() const; // return destination


	~Ticket(); // destructor

	private:

		std::string ticketId;
		std::string status;
		BaseUser user;
		date::date date;
		float price;
		Vehicle vehicle;
		std::string origin;
		std::string destination;

};
#endif
