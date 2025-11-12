#ifndef BASEVEHICLE_H
#define BASEVEHICLE_H

#include <string>

class BaseVehicle
{
private:
    int vehicleId, capacity;
    std::string vehicleType;

public:
    // default constuctor
    BaseVehicle() = default;

    // parameterized constuctor
    BaseVehicle(int vehicleId, const std::string &vehicleType, const int &capacity);

    // destructor
    virtual ~BaseVehicle() = default;

    // getters
    int getVehicleId() const;
    int getVehicleCapacity() const;
    std::string getVehicleType() const;

    // setters
    void setVehicleType(const std::string &vehicleType);
    void setCapacity(int capacity);
    void setVehicleId(int id);
};

#endif
