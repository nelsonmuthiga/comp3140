#ifndef AIRPLANE_H
#define AIRPLANE_H
#include <string>
#include "Vehicle.h"

class Airplane : public Vehicle {

    public:

    Airplane(); // default constructor

    Airplane(const std::string&, const std::string&, const int, const std::string&, const std::string& ,const std::string&); // parameterized constructor

    void setAirline(const std::string&); // set airplane id

    void setGateNo(const std::string&); // set model

    void setFlightNo(const std::string&); // set flight number

    std::string getAirline() const; // return airline

    std::string getGateNo() const; // return gate number

    std::string getFlightNo() const; // return flight number

    ~Airplane(); // destructor

    private:

        std::string airline;
        std::string gateNo;
        std::string flightNo;
};

#endif