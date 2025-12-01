#ifndef VEHICLESERVICE_H
#define VEHICLESERVICE_H

#include <memory>
#include <vector>
#include <string>
#include "BaseVehicle.h"

// Forward declarations
class Plane;
class Cab;
class Train;

/**
 * VehicleService handles all business logic related to vehicle management.
 * This service works with actual vehicle objects (OOP approach) rather than just data records.
 */
class VehicleService
{
public:
    VehicleService() = default;

    /**
     * Get all vehicles as objects
     * @return Vector of BaseVehicle pointers (polymorphic)
     */
    std::vector<std::shared_ptr<BaseVehicle>> getAllVehicleObjects() const;

    /**
     * Get vehicles of a specific type as objects
     * @param type The vehicle type ("Plane", "Cab", or "Train")
     * @return Vector of BaseVehicle pointers
     */
    std::vector<std::shared_ptr<BaseVehicle>> getVehiclesByType(const std::string &type) const;

    /**
     * Get a specific vehicle object by ID
     * @param vehicleId The vehicle database ID
     * @return Pointer to vehicle object, or nullptr if not found
     */
    std::shared_ptr<BaseVehicle> getVehicleById(int vehicleId) const;

    /**
     * Create a new Plane and store in database
     * @return The database ID of the created plane, or 0 if failed
     */
    int createPlane(const std::string &airline, const std::string &flightNumber,
                    const std::string &flightNo, int capacity);

    /**
     * Create a new Cab and store in database
     * @return The database ID of the created cab, or 0 if failed
     */
    int createCab(const std::string &licensePlate, const std::string &driverName, int capacity);

    /**
     * Create a new Train and store in database
     * @return The database ID of the created train, or 0 if failed
     */
    int createTrain(const std::string &trainNumber, const std::string &platform, int capacity);

    /**
     * Delete a vehicle from the database
     * @param vehicleId The vehicle ID to delete
     * @return true if successful
     */
    bool deleteVehicle(int vehicleId);

    /**
     * Display all vehicles in a formatted manner
     */
    void displayAllVehicles() const;

    /**
     * Get vehicle details as a formatted string
     * @param vehicle The vehicle object
     * @return Formatted string with vehicle details
     */
    std::string getVehicleDetails(std::shared_ptr<BaseVehicle> vehicle) const;
};

#endif
