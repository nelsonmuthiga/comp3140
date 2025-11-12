#include <iostream>
#include "Cab.h"

using namespace std;

// Cab class member function definitions

// Default constructor
Cab::Cab() : Vehicle(), driverName(""), licensePlate("") {}

// Parameterized constructor
Cab::Cab(const string& vehId, const string& vehType, const int cap, const string& driver,const string& license)
    : Vehicle(vehId, vehType, cap), driverName(driver), licensePlate(license)
{
}

// Setter methods
void Cab::setDriverName(const string& driver) {
    this->driverName = driver;
}

void Cab::setLicensePlate(const string& license) {
    this->licensePlate = license;
}

// Getter methods
string Cab::getDriverName() const {
    return driverName;
}

string Cab::getLicensePlate() const {
    return licensePlate;
}

Cab::~Cab() {
    // Destructor
}