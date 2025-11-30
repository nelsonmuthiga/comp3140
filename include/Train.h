#ifndef TRAIN_H
#define TRAIN_H

#include "BaseVehicle.h"
#include <string>

class Train : public BaseVehicle
{
private:
    std::string trainNumber;
    std::string platform;

public:
    // default constructor
    Train() = default;

    // parameterized constructor
    Train(int vehicleId, const std::string &trainNumber, const std::string &platform, int capacity);

    // getters
    std::string getTrainNumber() const;
    std::string getPlatform() const;

    // setters
    void setTrainNumber(const std::string &trainNumber);
    void setPlatform(const std::string &platform);
};

#endif