/**
 * UserInterface.h
 *
 * This file declares the UserInterface class, an abstract base class
 * that defines a common interface for different user interface types
 * in the timetabling system — specifically for Admin and Student interfaces.
 */

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>

 /**
  * UserInterface is an abstract base class that provides a common structure
  * for the different user interfaces in the system, such as the Admin and Student interfaces.
  */
class UserInterface {
public:
    /**
     * Creates a new UserInterface object.
     */
    UserInterface();

    /**
     * Destroys the UserInterface object.
     */
    virtual ~UserInterface();

    /**
     * Displays the main menu.
     *
     * This is a pure virtual function, meaning each derived class
     * must provide its own version to show the appropriate menu options.
     */
    virtual void displayMainMenu() const = 0;

    /**
     * Processes the user's menu selection.
     *
     * This pure virtual function must be implemented by derived classes
     * to handle the selected menu option.
     *
     * Returns true if the interface should continue running,
     * or false if the user chooses to exit.
     */
    virtual bool processMenuChoice(int choice) = 0;

    /**
     * Runs the user interface loop.
     *
     * Starts the interface and keeps it running until the user exits.
     * Returns true if everything completes successfully, or false if there’s an error.
     */
    virtual bool run() = 0;

    /**
     * Displays a header with a given title.
     */
    void displayHeader(const std::string& title) const;

    /**
     * Displays a footer.
     */
    void displayFooter() const;

    /**
     * Displays a success message.
     */
    void displaySuccess(const std::string& message) const;

    /**
     * Displays an error message.
     */
    void displayError(const std::string& message) const;

    /**
     * Displays a warning message.
     */
    void displayWarning(const std::string& message) const;

    /**
     * Displays an informational message.
     */
    void displayInfo(const std::string& message) const;

    /**
     * Waits for the user to press a key or hit enter to continue.
     */
    void waitForContinue() const;

    /**
     * Clears the screen.
     */
    void clearScreen() const;
};

#endif // USER_INTERFACE_H
