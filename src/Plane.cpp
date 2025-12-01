#include "Plane.h"

Plane::Plane(int vehicleId, const std::string &airline, const std::string &flightNumber,
             const std::string &flightNo, int capacity)
    : BaseVehicle(vehicleId, "Plane", capacity),
      airline(airline),
      flightNumber(flightNumber),
      flightNo(flightNo)
{
}

// Getters
std::string Plane::getAirline() const
{
    return airline;
}

std::string Plane::getFlightNumber() const
{
    return flightNumber;
}

std::string Plane::getFlightNo() const
{
    return flightNo;
}

// Setters
void Plane::setAirline(const std::string &airline)
{
    this->airline = airline;
}

void Plane::setFlightNumber(const std::string &flightNumber)
{
    this->flightNumber = flightNumber;
}

void Plane::setFlightNo(const std::string &flightNo)
{
    this->flightNo = flightNo;
}
