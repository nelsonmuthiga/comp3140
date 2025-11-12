#include <iostream>
#include "Airplane.h"

using namespace std;

// Airplane class member function definitions

// Default constructor
Airplane::Airplane() : Vehicle(), airline(""), gateNo(""), flightNo("") {}

// Parameterized constructor
Airplane::Airplane(const string& vehId, const string& vehType, const int cap, const string& airln,const string& gate,const string& flight)
    : Vehicle(vehId, vehType, cap), airline(airln), gateNo(gate), flightNo(flight)
{
}

// Setter methods
void Airplane::setAirline(const string& airln) {
    this->airline = airln;
}

void Airplane::setGateNo(const string& gate) {
    this->gateNo = gate;
}

void Airplane::setFlightNo(const string& flight) {
    this->flightNo = flight;
}

// Getter methods
string Airplane::getAirline() const {
    return airline;
}

string Airplane::getGateNo() const {
    return gateNo;
}

string Airplane::getFlightNo() const {
    return flightNo;
}

Airplane::~Airplane() {
    // Destructor
}