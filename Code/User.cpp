#include "User.h"
#include <iostream>

User::User(const std::string& name, const std::string& pass) 
    : username(name), password(pass) {}

std::string User::getUsername() const {
    return username;
}

bool User::verifyPassword(const std::string& pass) const {
    return password == pass;
}
