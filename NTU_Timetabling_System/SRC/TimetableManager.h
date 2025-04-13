/**
 * TimetableManager.h
 *
 * This file defines the TimetableManager class, which acts as the central
 * coordinator for the timetabling system. It manages modules, groups, students,
 * lecturers, rooms, and sessions, and handles saving and loading of data.
 */

#ifndef TIMETABLE_MANAGER_H
#define TIMETABLE_MANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <map>

 // Forward declarations for related classes
class Module;
class Group;
class SessionType;
class Room;
class Student;
class Lecturer;
class Session;
class Timetable;

// Main manager for the timetabling system, designed as a singleton
class TimetableManager {
public:
    // Returns the single instance of the TimetableManager
    static TimetableManager& getInstance();

    // Disable copy and assignment operations
    TimetableManager(const TimetableManager&) = delete;
    TimetableManager& operator=(const TimetableManager&) = delete;

    // Sets the academic year for the system
    void initialize(const std::string& academicYear);

    // Saves all data to the specified directory, returns true if successful
    bool saveData(const std::string& directory = "data");

    // Loads all data from the specified directory, returns true if successful
    bool loadData(const std::string& directory = "data");

    // Module management
    void addModule(const Module& module);
    const std::vector<Module>& getModules() const;
    Module* getModule(const std::string& code);
    Module& getModuleAt(int index);
    bool removeModule(int index);
    bool moduleExists(const std::string& code) const;

    // Group management
    void addGroup(const Group& group);
    const std::vector<Group>& getGroups() const;
    Group* getGroup(const std::string& code);
    Group& getGroupAt(int index);
    bool removeGroup(int index);
    bool groupExists(const std::string& code) const;

    // Session type management
    void addSessionType(const SessionType& sessionType);
    const std::vector<SessionType>& getSessionTypes() const;
    SessionType* getSessionType(const std::string& code);
    SessionType& getSessionTypeAt(int index);
    bool removeSessionType(int index);
    bool sessionTypeExists(const std::string& code) const;

    // Room management
    void addRoom(const Room& room);
    const std::vector<Room>& getRooms() const;
    Room* getRoom(const std::string& number);
    Room& getRoomAt(int index);
    bool removeRoom(int index);
    bool roomExists(const std::string& number) const;

    // Student management
    void addStudent(std::shared_ptr<Student> student);
    const std::vector<std::shared_ptr<Student>>& getStudents() const;
    std::shared_ptr<Student> getStudent(const std::string& studentId) const;
    std::shared_ptr<Student> getStudentByUsername(const std::string& username) const;
    std::shared_ptr<Student> getStudentAt(int index) const;
    bool removeStudent(const std::string& studentId);
    bool studentExists(const std::string& studentId) const;
    bool studentUsernameExists(const std::string& username) const;
    bool assignStudentToGroup(const std::string& studentId, const std::string& groupCode);

    // Lecturer management
    void addLecturer(std::shared_ptr<Lecturer> lecturer);
    const std::vector<std::shared_ptr<Lecturer>>& getLecturers() const;
    std::shared_ptr<Lecturer> getLecturer(const std::string& lecturerId) const;
    std::shared_ptr<Lecturer> getLecturerAt(int index) const;
    bool removeLecturer(const std::string& lecturerId);
    bool lecturerExists(const std::string& lecturerId) const;
    bool assignLecturerToModule(const std::string& lecturerId, const std::string& moduleCode);

    // Session management
    std::shared_ptr<Session> createSession(
        const std::string& moduleCode,
        const std::string& sessionTypeCode,
        const std::string& roomNumber,
        const std::string& lecturerId,
        const TimeSlot& timeSlot,
        int weekNumber,
        const std::vector<std::string>& groupCodes = {}
    );

    std::vector<std::shared_ptr<Session>> getAllSessions() const;
    std::vector<std::shared_ptr<Session>> getSessionsForGroup(const std::string& groupCode, int weekNumber) const;
    std::vector<std::shared_ptr<Session>> getSessionsForModuleAndGroup(const std::string& moduleCode, const std::string& groupCode) const;
    std::vector<std::shared_ptr<Session>> getSessionsForRoomAndGroup(const std::string& roomNumber, const std::string& groupCode) const;
    std::vector<std::shared_ptr<Session>> getSessionsForLecturerAndGroup(const std::string& lecturerId, const std::string& groupCode) const;

    // Finds any scheduling conflicts and returns pairs of conflicting sessions
    std::vector<std::pair<std::shared_ptr<Session>, std::shared_ptr<Session>>> findTimetableConflicts() const;

    // Authenticates a user with their username and password, returns the user if valid
    std::shared_ptr<User> authenticateUser(const std::string& username, const std::string& password) const;

private:
    // Private constructor and destructor for singleton pattern
    TimetableManager();
    ~TimetableManager();

    std::string m_academicYear;  // Current academic year

    std::vector<Module> m_modules;
    std::vector<Group> m_groups;
    std::vector<SessionType> m_sessionTypes;
    std::vector<Room> m_rooms;
    std::vector<std::shared_ptr<Student>> m_students;
    std::vector<std::shared_ptr<Lecturer>> m_lecturers;

    std::unique_ptr<Timetable> m_timetable;

    // Maps for fast lookups by identifier
    std::map<std::string, size_t> m_moduleMap;
    std::map<std::string, size_t> m_groupMap;
    std::map<std::string, size_t> m_sessionTypeMap;
    std::map<std::string, size_t> m_roomMap;

    std::map<std::string, std::shared_ptr<Student>> m_studentMap;
    std::map<std::string, std::shared_ptr<Student>> m_studentUsernameMap;
    std::map<std::string, std::shared_ptr<Lecturer>> m_lecturerMap;
};

#endif // TIMETABLE_MANAGER_H
