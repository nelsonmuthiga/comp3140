#ifndef USER_H
#define USER_H
#include <string> // C++ standard string class

class User {
public:

    User(); // default constructor

    User(const std::string&, const std::string&, const std::string&, const std::string&); // parameterized constructor

    User(const User&); // copy constructor

    void setUsername(const std::string&); // set username

    void setPassword(const std::string&); // set password

    void setFullname(const std::string&); // set full name

    void setEmail(const std::string&); // set email

    std::string getUsername() const; // return username

    std::string getPassword() const; // return password

    std::string getFullname() const; // return full name

    std::string getEmail() const; // return email

    void addUserToDatabase();

    bool checkUsernameAndPassword(const std::string&, const std::string&) const; // check username and password

    ~User(); // destructor

private:
    std::string username;
    std::string password;
    std::string fullname;
    std::string email;
};

#endif