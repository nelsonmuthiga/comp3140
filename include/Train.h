#ifndef TRAIN_H
#define TRAIN_H
#include <string>
#include "Vehicle.h"

class Train : public Vehicle {

    public:

    Train(); // default constructor

    Train(const std::string&, const std::string&, const int, const std::string&, const int&); // parameterized constructor

    void setTrainNo(const std::string&); // set railway company

    void setPlatform(const std::string&); // set number of carriages

    std::string getTrainNo() const; // return railway company

    std::string getPlatform() const; // return number of carriages

    ~Train(); // destructor

    private:

        std::string trainNo;
        std::string platform;
};
#endif