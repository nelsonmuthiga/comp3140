#ifndef CAB_H
#define CAB_H
#include <string>
#include "Vehicle.h"

class Cab : public Vehicle {

    public:

    Cab(); // default constructor

    Cab(const std::string&, const std::string&, const int, const std::string&, const std::string&); // parameterized constructor

    void setDriverName(const std::string&); // set driver name

    void setLicensePlate(const std::string&); // set license plate

    std::string getDriverName() const; // return driver name

    std::string getLicensePlate() const; // return license plate

    ~Cab(); // destructor

    private:

        std::string driverName;
        std::string licensePlate;
};
#endif