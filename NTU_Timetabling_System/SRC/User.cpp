/**
 *
 * This file contains the implementation of the base User class,
 * which defines common functionality for all user types in the system,
 * such as username management, password verification, and password updates.
 *
 * It also includes the factory function that creates specific user types
 * (Admin or Student) based on the provided user role.
 */

#include "User.h"
#include "Admin.h"
#include "Student.h"
#include <iostream>

 // Constructor implementation
User::User(const std::string& username, const std::string& password, UserType type)
    : m_username(username), m_password(password), m_userType(type) {
}

// Destructor implementation
User::~User() {
    // Nothing to clean up for now
}

// Get the username
const std::string& User::getUsername() const {
    return m_username;
}

// Get the user type
User::UserType User::getUserType() const {
    return m_userType;
}

// Verify the password
bool User::verifyPassword(const std::string& password) const {
    return m_password == password;
}

// Change the password
bool User::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!verifyPassword(oldPassword)) {
        return false;  // Old password didn't match
    }

    // Change the password
    m_password = newPassword;
    return true;
}

// Factory function to create a User of the specified type
std::unique_ptr<User> createUser(const std::string& username, const std::string& password, User::UserType type) {
    // Create a user of the specified type
    if (type == User::UserType::ADMIN) {
        return std::make_unique<Admin>(username, password);
    }
    else if (type == User::UserType::STUDENT) {
        return std::make_unique<Student>(username, password);
    }

    // If we get here, the type was invalid
    throw std::invalid_argument("Invalid user type");
}