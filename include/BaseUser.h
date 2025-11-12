#ifndef BASEUSER_H
#define BASEUSER_H

#include <string>
class BaseUser
{
private:
    // base parameters
    int id;
    std::string userName, password, fullName, email;

public:
    // default constructor
    BaseUser() = default;

    // parameterized constructor
    // using const reference because we are not modifying anything
    BaseUser(int id, const std::string &userName, const std::string &password, const std::string &fullName, const std::string &email);

    virtual ~BaseUser() = default;

    virtual void updateInfo();
    virtual bool login(const std::string &userName, const std::string &password);
    virtual bool logout();

    // getter
    int getId() const;
    std::string getUserName() const;
    std::string getPassword() const;
    std::string getFullName() const;
    std::string getEmail() const;

    // setter
    void setUserName(const std::string &userName);
    void setPassword(const std::string &password);
    void setFullName(const std::string &fullName);
    void setEmail(const std::string &email);
    void setId(int id);
};

#endif