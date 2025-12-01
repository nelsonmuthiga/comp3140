#include "Cab.h"

Cab::Cab(int vehicleId, const std::string &licensePlate, const std::string &driverName, int capacity)
    : BaseVehicle(vehicleId, "Cab", capacity),
      licensePlate(licensePlate),
      driverName(driverName)
{
}

// Getters
std::string Cab::getLicensePlate() const
{
    return licensePlate;
}

std::string Cab::getDriverName() const
{
    return driverName;
}

// Setters
void Cab::setLicensePlate(const std::string &licensePlate)
{
    this->licensePlate = licensePlate;
}

void Cab::setDriverName(const std::string &driverName)
{
    this->driverName = driverName;
}
