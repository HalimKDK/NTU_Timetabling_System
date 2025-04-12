/**
  *
 * This file defines the SessionType class, which represents different kinds
 * of academic sessions in the timetabling system — like lectures, labs,
 * or tutorials. It includes information such as a short code, a descriptive name,
 * and an optional description for each session type.
 */

#ifndef SESSION_TYPE_H
#define SESSION_TYPE_H

#include <string>

 /**
  * Represents a type of academic session.
  *
  * A SessionType could be something like a Lecture, a Laboratory, or a Tutorial.
  * It stores a short code, a descriptive name, and an optional description.
  */
class SessionType {
public:
    /**
     * Creates a new SessionType object with a code, a name, and an optional description.
     * For example, code "LEC", name "Lecture", description "A standard teaching session."
     */
    SessionType(const std::string& code, const std::string& name, const std::string& description = "");

    // Destructor — cleans up the SessionType object
    ~SessionType();

    // Returns the short code for this session type
    const std::string& getCode() const;

    // Updates the short code for this session type
    void setCode(const std::string& code);

    // Returns the name of the session type
    const std::string& getName() const;

    // Updates the name of the session type
    void setName(const std::string& name);

    // Returns the description of the session type
    const std::string& getDescription() const;

    // Updates the description of the session type
    void setDescription(const std::string& description);

private:
    std::string m_code;          // Short code for the session type (e.g., "LEC", "LAB")
    std::string m_name;          // Descriptive name for the session type (e.g., "Lecture")
    std::string m_description;   // Optional description of the session type
};

#endif // SESSION_TYPE_H

