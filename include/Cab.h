#ifndef CAB_H
#define CAB_H

#include "BaseVehicle.h"
#include <string>

class Cab : public BaseVehicle
{
private:
    std::string licensePlate;
    std::string driverName;

public:
    // default constructor
    Cab() = default;

    // parameterized constructor
    Cab(int vehicleId, const std::string &licensePlate, const std::string &driverName, int capacity);

    // getters
    std::string getLicensePlate() const;
    std::string getDriverName() const;

    // setters
    void setLicensePlate(const std::string &licensePlate);
    void setDriverName(const std::string &driverName);
};

#endif