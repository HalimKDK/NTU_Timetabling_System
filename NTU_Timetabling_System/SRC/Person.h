/**
 *
 * This file defines the Person class, which acts as a base class for all types of people
 * in the timetabling system — including students and lecturers. It provides common
 * attributes like first name, last name, and ID, along with methods to access and
 * modify these details.
 */
#ifndef PERSON_H
#define PERSON_H
#include <string>

 /**
  * Represents a person in the timetabling system.
  *
  * This class defines common attributes and methods shared by all people,
  * such as students and lecturers.
  */
class Person {
public:
    // Default constructor for STL compatibility
    Person() : m_firstName(""), m_lastName(""), m_id("") {}

    // Creates a new person with a first name, last name, and optional ID
    Person(const std::string& firstName, const std::string& lastName, const std::string& id = "");

    // Copy and move operations
    Person(const Person&) = default;
    Person(Person&&) = default;
    Person& operator=(const Person&) = default;
    Person& operator=(Person&&) = default;

    // Virtual destructor for proper cleanup in derived classes
    virtual ~Person();

    // Returns the person's first name
    const std::string& getFirstName() const;
    // Sets a new first name for the person
    void setFirstName(const std::string& firstName);
    // Returns the person's last name
    const std::string& getLastName() const;
    // Sets a new last name for the person
    void setLastName(const std::string& lastName);
    // Returns the full name (first + last)
    std::string getFullName() const;
    // Returns the person's ID
    const std::string& getId() const;
    // Sets a new ID for the person
    void setId(const std::string& id);
protected:
    std::string m_firstName;  // The person's first name
    std::string m_lastName;   // The person's last name
    std::string m_id;         // Unique identifier for the person
};
#endif // PERSON_H