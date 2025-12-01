#include "VehicleService.h"
#include "Database.h"
#include "Plane.h"
#include "Cab.h"
#include "Train.h"
#include <iostream>
#include <sstream>

std::vector<std::shared_ptr<BaseVehicle>> VehicleService::getAllVehicleObjects() const
{
    std::vector<std::shared_ptr<BaseVehicle>> vehicles;

    // Get all vehicle records from database
    auto records = Database::getInstance().getAllVehicles();

    // Convert each record to an actual object
    for (const auto &record : records)
    {
        auto vehicle = Database::getInstance().createVehicleObject(record);
        if (vehicle)
        {
            vehicles.push_back(vehicle);
        }
    }

    return vehicles;
}

std::vector<std::shared_ptr<BaseVehicle>> VehicleService::getVehiclesByType(const std::string &type) const
{
    std::vector<std::shared_ptr<BaseVehicle>> vehicles;

    // Get vehicle records of specific type from database
    auto records = Database::getInstance().getVehiclesByType(type);

    // Convert each record to an actual object
    for (const auto &record : records)
    {
        auto vehicle = Database::getInstance().createVehicleObject(record);
        if (vehicle)
        {
            vehicles.push_back(vehicle);
        }
    }

    return vehicles;
}

std::shared_ptr<BaseVehicle> VehicleService::getVehicleById(int vehicleId) const
{
    // Get vehicle record from database
    auto record = Database::getInstance().getVehicleById(vehicleId);

    if (record.id == 0)
    {
        return nullptr; // Not found
    }

    // Convert record to object
    return Database::getInstance().createVehicleObject(record);
}

int VehicleService::createPlane(const std::string &airline, const std::string &flightNumber,
                                const std::string &flightNo, int capacity)
{
    // Step 1: Create the actual Plane object (using your Plane class!)
    Plane newPlane(0, airline, flightNumber, flightNo, capacity);

    // Step 2: Create a record to store in database
    VehicleRecord record;
    record.vehicleType = newPlane.getVehicleType();  // Get from Plane object
    record.capacity = newPlane.getVehicleCapacity(); // Get from Plane object
    record.airline = newPlane.getAirline();          // Get from Plane object
    record.flightNumber = newPlane.getFlightNumber();
    record.flightNo = newPlane.getFlightNo();

    // Step 3: Store in database
    if (Database::getInstance().createVehicle(record))
    {
        // Get the ID of the newly created vehicle
        auto vehicles = Database::getInstance().getVehiclesByType("Plane");
        if (!vehicles.empty())
        {
            return vehicles.back().id;
        }
    }

    return 0; // Failed
}

int VehicleService::createCab(const std::string &licensePlate, const std::string &driverName, int capacity)
{
    // Step 1: Create the actual Cab object (using your Cab class!)
    Cab newCab(0, licensePlate, driverName, capacity);

    // Step 2: Create a record to store in database
    VehicleRecord record;
    record.vehicleType = newCab.getVehicleType();   // Get from Cab object
    record.capacity = newCab.getVehicleCapacity();  // Get from Cab object
    record.licensePlate = newCab.getLicensePlate(); // Get from Cab object
    record.driverName = newCab.getDriverName();     // Get from Cab object

    // Step 3: Store in database
    if (Database::getInstance().createVehicle(record))
    {
        // Get the ID of the newly created vehicle
        auto vehicles = Database::getInstance().getVehiclesByType("Cab");
        if (!vehicles.empty())
        {
            return vehicles.back().id;
        }
    }

    return 0; // Failed
}

int VehicleService::createTrain(const std::string &trainNumber, const std::string &platform, int capacity)
{
    // Step 1: Create the actual Train object (using your Train class!)
    Train newTrain(0, trainNumber, platform, capacity);

    // Step 2: Create a record to store in database
    VehicleRecord record;
    record.vehicleType = newTrain.getVehicleType();  // Get from Train object
    record.capacity = newTrain.getVehicleCapacity(); // Get from Train object
    record.trainNumber = newTrain.getTrainNumber();  // Get from Train object
    record.platform = newTrain.getPlatform();        // Get from Train object

    // Step 3: Store in database
    if (Database::getInstance().createVehicle(record))
    {
        // Get the ID of the newly created vehicle
        auto vehicles = Database::getInstance().getVehiclesByType("Train");
        if (!vehicles.empty())
        {
            return vehicles.back().id;
        }
    }

    return 0; // Failed
}

bool VehicleService::deleteVehicle(int vehicleId)
{
    return Database::getInstance().deleteVehicle(vehicleId);
}

void VehicleService::displayAllVehicles() const
{
    auto vehicles = getAllVehicleObjects();

    std::cout << "\n--- All Vehicles (OOP View) ---" << std::endl;
    if (vehicles.empty())
    {
        std::cout << "No vehicles found." << std::endl;
        return;
    }

    for (const auto &vehicle : vehicles)
    {
        std::cout << getVehicleDetails(vehicle) << std::endl;
    }
}

std::string VehicleService::getVehicleDetails(std::shared_ptr<BaseVehicle> vehicle) const
{
    if (!vehicle)
    {
        return "Invalid vehicle";
    }

    std::ostringstream details;
    details << "ID: " << vehicle->getVehicleId()
            << " | Type: " << vehicle->getVehicleType()
            << " | Capacity: " << vehicle->getVehicleCapacity();

    // Simple approach: Check type string and cast accordingly
    std::string type = vehicle->getVehicleType();

    if (type == "Plane")
    {
        // Cast to Plane to access Plane-specific methods
        Plane *plane = dynamic_cast<Plane *>(vehicle.get());
        if (plane)
        {
            details << " | Airline: " << plane->getAirline()
                    << " | Flight: " << plane->getFlightNumber();
        }
    }
    else if (type == "Cab")
    {
        // Cast to Cab to access Cab-specific methods
        Cab *cab = dynamic_cast<Cab *>(vehicle.get());
        if (cab)
        {
            details << " | License: " << cab->getLicensePlate()
                    << " | Driver: " << cab->getDriverName();
        }
    }
    else if (type == "Train")
    {
        // Cast to Train to access Train-specific methods
        Train *train = dynamic_cast<Train *>(vehicle.get());
        if (train)
        {
            details << " | Train#: " << train->getTrainNumber()
                    << " | Platform: " << train->getPlatform();
        }
    }

    return details.str();
}
