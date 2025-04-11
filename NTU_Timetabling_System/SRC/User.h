/**
 * This file contains the declaration of the User class which serves
 * as a base class for all user types (Admin and Student)
 */

#ifndef USER_H
#define USER_H

#include <string>
#include <memory>

// The User class is a base class for all types of users in the system,
// like Admins and Students. It defines the shared behavior and properties.
class User {
public:

    // Types of users in the system.
    enum class UserType {
        ADMIN,
        STUDENT
    };

    // Creates a new user with a username, password, and user type.
    User(const std::string& username, const std::string& password, UserType type);

    // Virtual destructor to ensure proper cleanup in derived classes.
    virtual ~User();

    // Returns the username of the user.
    const std::string& getUsername() const;

    // Returns whether this user is an admin or a student.
    UserType getUserType() const;

    // Use of bool to check if the given password matches the user's password.
    bool verifyPassword(const std::string& password) const;

    // Tries to change the user's password. Returns true if successful.
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);

    // Shows the main menu for the user.
    // This function must be implemented by any class that inherits from User.
    virtual void displayMenu() const = 0;

protected:
    std::string m_username; ///< This variable stores the users username
    std::string m_password; ///< This variable stores the users password
    UserType m_userType;    ///< This represents the type of user (admin or student)
};

// I added a helper function to create a user based on type.
// Returns a smart pointer to a new User object.
std::unique_ptr<User> createUser(const std::string& username, const std::string& password, User::UserType type);

#endif // USER_H