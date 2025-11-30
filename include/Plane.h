#ifndef PLANE_H
#define PLANE_H

#include "BaseVehicle.h"
#include <string>

class Plane : public BaseVehicle
{
private:
    std::string airline;
    std::string flightNumber;
    std::string flightNo;

public:
    // default constructor
    Plane() = default;

    // parameterized constructor
    Plane(int vehicleId, const std::string &airline, const std::string &flightNumber,
          const std::string &flightNo, int capacity);

    // getters
    std::string getAirline() const;
    std::string getFlightNumber() const;
    std::string getFlightNo() const;

    // setters
    void setAirline(const std::string &airline);
    void setFlightNumber(const std::string &flightNumber);
    void setFlightNo(const std::string &flightNo);
};
#endif