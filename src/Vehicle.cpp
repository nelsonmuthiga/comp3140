#include <iostream>
#include "Vehicle.h"

using namespace std;

// Vehicle class member function definitions

// Default constructor
Vehicle::Vehicle() : id(""), type(""), capacity(0) {}

// Parameterized constructor
Vehicle::Vehicle(const string& vehId, const string& vehType, int cap)
    : id(vehId), type(vehType), capacity(cap)
{
}

// Setter methods
void Vehicle::setVehicleId(const string& vehId) {
    this->id = vehId;
}

void Vehicle::setType(const string& vehType) {
    this->type = vehType;
}

void Vehicle::setCapacity(double cap) {
    this->capacity = cap;
}

// Getter methods
string Vehicle::getVehicleId() const {
    return id;
}

string Vehicle::getVehicleType() const {
    return type;
}

double Vehicle::getVehicleCapacity() const {
    return capacity;
}

Vehicle::~Vehicle() {
    // Destructor
}