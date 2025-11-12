#include <iostream>
#include "Train.h"

using namespace std;

// Train class member function definitions

// Default constructor
Train::Train() : Vehicle(), trainNo(""), platform("") {}

// Parameterized constructor
Train::Train(const string& vehId, const string& vehType, const int cap, const string& train,const int& plat)
    : Vehicle(vehId, vehType, cap), trainNo(train), platform(to_string(plat))
{
}

// Setter methods
void Train::setTrainNo(const string& train) {
    this->trainNo = train;
}

void Train::setPlatform(const string& plat) {
    this->platform = plat;
}

// Getter methods
string Train::getTrainNo() const {
    return trainNo;
}

string Train::getPlatform() const {
    return platform;
}

Train::~Train() {
    // Destructor
}