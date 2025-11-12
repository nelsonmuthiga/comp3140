#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>


class Vehicle {

	public:

	Vehicle(); // default constructor

	Vehicle(const std::string&, const std::string&, int); // parameterized constructor

	void setVehicleId(const std::string&); // set event name

	void setType(const std::string&); // set date

	void setCapacity(double); // set price

	std::string getVehicleId() const; // return company name

	std::string getVehicleType() const; // return date

	double getVehicleCapacity() const; // return price

    ~Vehicle(); // destructor

	private:

		std::string id;
        std::string type;
        int capacity;
};
#endif
