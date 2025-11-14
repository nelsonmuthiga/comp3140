#include "Customer.h"
#include <algorithm>

// Parameterized constructor
Customer::Customer(int id, const std::string &userName, const std::string &password,
                   const std::string &fullName, const std::string &email,
                   const std::string &phone)
    : BaseUser(id, userName, password, fullName, email), phone(phone) {}

// Simple state management - no business logic
std::string Customer::getPhone() const
{
    return phone;
}

void Customer::setPhone(const std::string &phone)
{
    this->phone = phone;
}

std::vector<std::string> Customer::getBookingIds() const
{
    return bookingIds;
}

void Customer::addBookingId(const std::string &bookingId)
{
    bookingIds.push_back(bookingId);
}

void Customer::removeBookingId(const std::string &bookingId)
{
    auto it = std::find(bookingIds.begin(), bookingIds.end(), bookingId);
    if (it != bookingIds.end())
    {
        bookingIds.erase(it);
    }
}
