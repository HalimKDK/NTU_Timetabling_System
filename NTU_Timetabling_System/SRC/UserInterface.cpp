/**
 * UserInterface.cpp
 *
 * This file contains the implementation of the UserInterface class,
 * which provides a set of common functions used by both the Admin
 * and Student interfaces in the timetabling system.
 */

#include "UserInterface.h"
#include <iostream>
#include <string>
#include <limits>

 // Constructor
UserInterface::UserInterface() {
    // Nothing needs to be set up when the object is created
}

// Destructor
UserInterface::~UserInterface() {
    // Nothing to clean up when the object is destroyed
}

// Displays a formatted header with the given title centered on the screen
void UserInterface::displayHeader(const std::string& title) const {
    int padding = (50 - static_cast<int>(title.length())) / 2;

    std::cout << std::string(50, '=') << std::endl;
    std::cout << std::string(padding, ' ') << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << std::endl;
}

// Displays a simple footer line to visually close a section
void UserInterface::displayFooter() const {
    std::cout << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << std::endl;
}

// Shows a success message with a label
void UserInterface::displaySuccess(const std::string& message) const {
    std::cout << "\n[SUCCESS] " << message << std::endl;
}

// Shows an error message with a label
void UserInterface::displayError(const std::string& message) const {
    std::cout << "\n[ERROR] " << message << std::endl;
}

// Shows a warning message with a label
void UserInterface::displayWarning(const std::string& message) const {
    std::cout << "\n[WARNING] " << message << std::endl;
}

// Shows an informational message with a label
void UserInterface::displayInfo(const std::string& message) const {
    std::cout << "\n[INFO] " << message << std::endl;
}

// Waits for the user to press Enter before continuing
void UserInterface::waitForContinue() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Clears the console screen
void UserInterface::clearScreen() const {
    // Clears the screen depending on the platform
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
