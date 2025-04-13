/**
 * Session.h
 *
 * This file declares the Session class, which represents a scheduled
 * session (like a lecture, seminar, or lab) in the timetabling system.
 */

#ifndef SESSION_H
#define SESSION_H

#include "TimeSlot.h"
#include <string>
#include <memory>
#include <vector>

 // Forward declarations for other classes we’ll use by reference or pointer
class Module;
class SessionType;
class Room;
class Lecturer;
class Group;

/**
 * This class represents a scheduled session in the timetable.
 * A session is linked to a module, a type (like lecture or lab),
 * a room, a lecturer, a time slot, one or more groups, and the week number.
 */
class Session {
public:
    // Constructor to create a new Session.
    Session(const std::string& id,
        const Module& module,
        const SessionType& sessionType,
        const Room& room,
        const Lecturer& lecturer,
        const TimeSlot& timeSlot,
        int weekNumber);

    // Destructor
    ~Session();

    // Get the session’s unique ID
    const std::string& getId() const;

    // Change the session’s unique ID
    void setId(const std::string& id);

    // Get the module this session belongs to
    const Module& getModule() const;

    // Set a different module for this session
    void setModule(const Module& module);

    // Get the type of session (lecture, lab, etc.)
    const SessionType& getSessionType() const;

    // Set a different session type
    void setSessionType(const SessionType& sessionType);

    // Get the room where this session happens
    const Room& getRoom() const;

    // Set a different room for this session
    void setRoom(const Room& room);

    // Get the lecturer leading this session
    const Lecturer& getLecturer() const;

    // Set a different lecturer
    void setLecturer(const Lecturer& lecturer);

    // Get the time slot this session takes place in
    const TimeSlot& getTimeSlot() const;

    // Change the time slot for this session
    void setTimeSlot(const TimeSlot& timeSlot);

    // Get the week number (1-53) when this session occurs
    int getWeekNumber() const;

    // Set a new week number
    void setWeekNumber(int weekNumber);

    // Add a group to this session
    void addGroup(std::shared_ptr<Group> group);

    // Remove a group by its group code.
    // Returns true if removed successfully, false if it wasn’t found.
    bool removeGroup(const std::string& groupCode);

    // Get a list of all groups assigned to this session
    const std::vector<std::shared_ptr<Group>>& getGroups() const;

    // Check if a group is assigned to this session by its code.
    // Returns true if found, false otherwise.
    bool hasGroup(const std::string& groupCode) const;

    // Check if this session clashes with another session (same time, same day, overlapping)
    // Returns true if there’s a conflict, false if they don’t
    bool conflictsWith(const Session& other) const;

    // Get a readable string description of the session
    std::string toString() const;

private:
    std::string m_id;                       // Unique identifier for the session
    const Module& m_module;                 // The module this session belongs to
    const SessionType& m_sessionType;       // Type of session (Lecture, Lab, etc.)
    const Room& m_room;                     // Room where the session takes place
    const Lecturer& m_lecturer;             // Lecturer leading the session
    TimeSlot m_timeSlot;                    // Time slot for the session
    int m_weekNumber;                       // Week number (1-53)
    std::vector<std::shared_ptr<Group>> m_groups; // Groups assigned to the session
};

#endif // SESSION_H

