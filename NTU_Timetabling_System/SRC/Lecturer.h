/**
 *
 * This file defines the Lecturer class for the timetabling system.
 * A lecturer can teach multiple modules and sessions, and this class
 * manages their basic details like name, title, department, and the
 * modules/sessions they are responsible for.
 */
#ifndef LECTURER_H
#define LECTURER_H
#include "Person.h"
#include <string>
#include <vector>
#include <memory>

 // Forward declarations to avoid circular includes
class Module;
class Session;

/**
 * Represents a lecturer in the system.
 *
 * A Lecturer is a type of Person who can teach modules and sessions.
 */
class Lecturer : public Person {
public:
    // Default constructor for STL compatibility
    Lecturer() : Person(), m_title(""), m_department("") {}

    // Creates a new Lecturer with a name, ID, optional title, and department
    Lecturer(const std::string& firstName, const std::string& lastName,
        const std::string& id, const std::string& title = "",
        const std::string& department = "");

    // Constructor specifically for STL operations with 7 string arguments
    Lecturer(const std::string& firstName, const std::string& lastName,
        const std::string& id, const std::string& title,
        const std::string& department, const std::string& extra1,
        const std::string& extra2);

    // Explicitly declare copy/move constructors and assignment operators
    Lecturer(const Lecturer&) = default;
    Lecturer(Lecturer&&) = default;
    Lecturer& operator=(const Lecturer&) = default;
    Lecturer& operator=(Lecturer&&) = default;

    // Virtual destructor
    virtual ~Lecturer();

    // Returns the lecturer's title
    const std::string& getTitle() const;

    // Sets the lecturer's title
    void setTitle(const std::string& title);

    // Returns the lecturer's full name, including title (e.g., "Dr. John Smith")
    std::string getFormattedName() const;

    // Returns the department the lecturer belongs to
    const std::string& getDepartment() const;

    // Sets the lecturer's department
    void setDepartment(const std::string& department);

    // Adds a module to the lecturer's list of modules taught
    void addModule(std::shared_ptr<Module> module);

    // Removes a module from the lecturer's list, by module code
    bool removeModule(const std::string& moduleCode);

    // Returns the list of modules this lecturer teaches
    const std::vector<std::shared_ptr<Module>>& getModules() const;

    // Checks if the lecturer teaches a module with a specific code
    bool teachesModule(const std::string& moduleCode) const;

    // Adds a session to the lecturer's list of sessions taught
    void addSession(std::shared_ptr<Session> session);

    // Removes a session from the lecturer's list, by session ID
    bool removeSession(const std::string& sessionId);

    // Returns the list of sessions this lecturer teaches
    const std::vector<std::shared_ptr<Session>>& getSessions() const;

    // Checks if the lecturer teaches a session with a specific ID
    bool teachesSession(const std::string& sessionId) const;

private:
    std::string m_title;      // Lecturer's title (like "Dr", "Prof")
    std::string m_department; // Lecturer's department
    std::vector<std::shared_ptr<Module>> m_modules;   // Modules taught by this lecturer
    std::vector<std::shared_ptr<Session>> m_sessions; // Sessions taught by this lecturer
};

#endif // LECTURER_H