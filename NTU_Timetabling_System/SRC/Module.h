/**
 *
 * This file defines the Module class, which represents an academic module
 * in the timetabling system. A module has a unique code, a name, and one or more
 * lecturers assigned to it. The class provides functionality to manage these
 * attributes and relationships, allowing the system to associate sessions with
 * specific modules and lecturers.
 */

#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>
#include <memory>

 // Forward declaration
class Lecturer;

/**
 * Represents an academic module, such as a course or subject.
 * A module has a code, a name, and a list of lecturers teaching it.
 */
class Module {
public:
    // Creates a module with a given code and name
    Module(const std::string& code, const std::string& name);

    // Default destructor
    ~Module();

    // Returns the module code (e.g., SOFT20091)
    const std::string& getCode() const;

    // Updates the module code
    void setCode(const std::string& code);

    // Returns the module name (e.g., Software Design and Implementation)
    const std::string& getName() const;

    // Updates the module name
    void setName(const std::string& name);

    // Assigns a lecturer to this module
    void addLecturer(std::shared_ptr<Lecturer> lecturer);

    // Removes a lecturer by ID, returns true if successful
    bool removeLecturer(const std::string& lecturerId);

    // Returns a list of all lecturers assigned to this module
    const std::vector<std::shared_ptr<Lecturer>>& getLecturers() const;

    // Checks if a lecturer with the given ID is assigned to this module
    bool hasLecturer(const std::string& lecturerId) const;

private:
    std::string m_code;   // Module code (e.g., SOFT20091)
    std::string m_name;   // Module name  (e.g., Software Design and Implementation)
    std::vector<std::shared_ptr<Lecturer>> m_lecturers; // Assigned lecturers
};

#endif // MODULE_H

