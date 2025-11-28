#pragma once
#include <string>

class Grid;

class User {
protected:
    std::string username;
    std::string password;

public:
    User(const std::string& name, const std::string& pass);
    virtual ~User() = default;
    virtual void viewHeatmap(const Grid& grid) = 0;
    virtual void displayMenu() = 0;
    std::string getUsername() const;
    bool verifyPassword(const std::string& pass) const;
};