#include "BaseVehicle.h"

BaseVehicle::BaseVehicle(int vehicleId, const std::string &vehicleType, const int &capacity)
    : vehicleId(vehicleId), vehicleType(vehicleType), capacity(capacity)
{
}

// Getters
int BaseVehicle::getVehicleId() const
{
    return vehicleId;
}

int BaseVehicle::getVehicleCapacity() const
{
    return capacity;
}

std::string BaseVehicle::getVehicleType() const
{
    return vehicleType;
}

// Setters
void BaseVehicle::setVehicleType(const std::string &vehicleType)
{
    this->vehicleType = vehicleType;
}

void BaseVehicle::setCapacity(int capacity)
{
    this->capacity = capacity;
}

void BaseVehicle::setVehicleId(int id)
{
    this->vehicleId = id;
}
