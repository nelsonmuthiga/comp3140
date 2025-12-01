#include "Train.h"

Train::Train(int vehicleId, const std::string &trainNumber, const std::string &platform, int capacity)
    : BaseVehicle(vehicleId, "Train", capacity),
      trainNumber(trainNumber),
      platform(platform)
{
}

// Getters
std::string Train::getTrainNumber() const
{
    return trainNumber;
}

std::string Train::getPlatform() const
{
    return platform;
}

// Setters
void Train::setTrainNumber(const std::string &trainNumber)
{
    this->trainNumber = trainNumber;
}

void Train::setPlatform(const std::string &platform)
{
    this->platform = platform;
}
