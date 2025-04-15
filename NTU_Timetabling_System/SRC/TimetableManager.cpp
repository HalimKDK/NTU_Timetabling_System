/**
 *
 * This file implements the TimetableManager class, which acts as the central hub
 * for managing the academic timetable system. It handles operations like adding
 * and removing modules, groups, rooms, session types, students, and lecturers,
 * as well as creating sessions and saving or loading timetable data.
 */

#include "TimetableManager.h"
#include "Module.h"
#include "Group.h"
#include "SessionType.h"
#include "Room.h"
#include "Student.h"
#include "Lecturer.h"
#include "Session.h"
#include "Timetable.h"
#include "User.h"
#include "Admin.h"  // Added Admin include
#include "FileManager.h"
#include "TimeSlot.h"
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>

 // Singleton instance
TimetableManager& TimetableManager::getInstance() {
    static TimetableManager instance;
    return instance;
}

// Constructor
TimetableManager::TimetableManager() {
    // Initialize with default academic year
    initialize("2024-2025");
}

// Destructor
TimetableManager::~TimetableManager() {
    // Nothing specific to clean up, smart pointers handle memory
}

// Initialize with a new academic year
void TimetableManager::initialize(const std::string& academicYear) {
    m_academicYear = academicYear;

    // Clear all existing data
    m_modules.clear();
    m_groups.clear();
    m_sessionTypes.clear();
    m_rooms.clear();
    m_students.clear();
    m_lecturers.clear();
    m_admins.clear();  // Clear admins

    m_moduleMap.clear();
    m_groupMap.clear();
    m_sessionTypeMap.clear();
    m_roomMap.clear();
    m_studentMap.clear();
    m_studentUsernameMap.clear();
    m_lecturerMap.clear();
    m_adminMap.clear();  // Clear admin map
    m_userMap.clear();

    // Create a new timetable
    m_timetable = std::make_unique<Timetable>(academicYear);

    // Add default session types
    addSessionType(SessionType("LEC", "Lecture", "Standard lecture session"));
    addSessionType(SessionType("LAB", "Laboratory", "Practical laboratory session"));
    addSessionType(SessionType("TUT", "Tutorial", "Small group tutorial"));
    addSessionType(SessionType("SEM", "Seminar", "Discussion-based seminar"));
    addSessionType(SessionType("WRK", "Workshop", "Interactive workshop"));
}

// Save all data to files
bool TimetableManager::saveData(const std::string& directory) {
    try {
        FileManager fileManager(directory);
        return fileManager.saveData(*this);
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving data: " << e.what() << std::endl;
        return false;
    }
}

// Load all data from files
bool TimetableManager::loadData(const std::string& directory) {
    try {
        FileManager fileManager(directory);
        return fileManager.loadData(*this);
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
        return false;
    }
}

// Module management
void TimetableManager::addModule(const Module& module) {
    // Check if module already exists
    if (moduleExists(module.getCode())) {
        throw std::invalid_argument("Module with code " + module.getCode() + " already exists");
    }

    // Add module to vector and map
    size_t index = m_modules.size();
    m_modules.push_back(module);
    m_moduleMap[module.getCode()] = index;
}

const std::vector<Module>& TimetableManager::getModules() const {
    return m_modules;
}

Module* TimetableManager::getModule(const std::string& code) {
    auto it = m_moduleMap.find(code);
    if (it != m_moduleMap.end()) {
        return &m_modules[it->second];
    }

    return nullptr;
}

Module& TimetableManager::getModuleAt(int index) {
    if (index < 0 || index >= static_cast<int>(m_modules.size())) {
        throw std::out_of_range("Module index out of range");
    }

    return m_modules[index];
}

bool TimetableManager::removeModule(int index) {
    if (index < 0 || index >= static_cast<int>(m_modules.size())) {
        return false;
    }

    // Get the module code before removing
    std::string code = m_modules[index].getCode();

    // Remove module from vector
    m_modules.erase(m_modules.begin() + index);

    // Remove from map
    m_moduleMap.erase(code);

    // Update remaining indices in map
    for (auto it = m_moduleMap.begin(); it != m_moduleMap.end(); ++it) {
        if (it->second > static_cast<size_t>(index)) {
            it->second--;
        }
    }

    return true;
}

bool TimetableManager::moduleExists(const std::string& code) const {
    return m_moduleMap.find(code) != m_moduleMap.end();
}

// Group management
void TimetableManager::addGroup(const Group& group) {
    // Check if group already exists
    if (groupExists(group.getCode())) {
        throw std::invalid_argument("Group with code " + group.getCode() + " already exists");
    }

    // Add group to vector and map
    size_t index = m_groups.size();
    m_groups.push_back(group);
    m_groupMap[group.getCode()] = index;
}

const std::vector<Group>& TimetableManager::getGroups() const {
    return m_groups;
}

Group* TimetableManager::getGroup(const std::string& code) {
    auto it = m_groupMap.find(code);
    if (it != m_groupMap.end()) {
        return &m_groups[it->second];
    }

    return nullptr;
}

Group& TimetableManager::getGroupAt(int index) {
    if (index < 0 || index >= static_cast<int>(m_groups.size())) {
        throw std::out_of_range("Group index out of range");
    }

    return m_groups[index];
}

bool TimetableManager::removeGroup(int index) {
    if (index < 0 || index >= static_cast<int>(m_groups.size())) {
        return false;
    }

    // Get the group code before removing
    std::string code = m_groups[index].getCode();

    // Remove group from vector
    m_groups.erase(m_groups.begin() + index);

    // Remove from map
    m_groupMap.erase(code);

    // Update remaining indices in map
    for (auto it = m_groupMap.begin(); it != m_groupMap.end(); ++it) {
        if (it->second > static_cast<size_t>(index)) {
            it->second--;
        }
    }

    return true;
}

bool TimetableManager::groupExists(const std::string& code) const {
    return m_groupMap.find(code) != m_groupMap.end();
}

// Session type management
void TimetableManager::addSessionType(const SessionType& sessionType) {
    // Check if session type already exists
    if (sessionTypeExists(sessionType.getCode())) {
        throw std::invalid_argument("Session type with code " + sessionType.getCode() + " already exists");
    }

    // Add session type to vector and map
    size_t index = m_sessionTypes.size();
    m_sessionTypes.push_back(sessionType);
    m_sessionTypeMap[sessionType.getCode()] = index;
}

const std::vector<SessionType>& TimetableManager::getSessionTypes() const {
    return m_sessionTypes;
}

SessionType* TimetableManager::getSessionType(const std::string& code) {
    auto it = m_sessionTypeMap.find(code);
    if (it != m_sessionTypeMap.end()) {
        return &m_sessionTypes[it->second];
    }

    return nullptr;
}

SessionType& TimetableManager::getSessionTypeAt(int index) {
    if (index < 0 || index >= static_cast<int>(m_sessionTypes.size())) {
        throw std::out_of_range("Session type index out of range");
    }

    return m_sessionTypes[index];
}

bool TimetableManager::removeSessionType(int index) {
    if (index < 0 || index >= static_cast<int>(m_sessionTypes.size())) {
        return false;
    }

    // Get the session type code before removing
    std::string code = m_sessionTypes[index].getCode();

    // Remove session type from vector
    m_sessionTypes.erase(m_sessionTypes.begin() + index);

    // Remove from map
    m_sessionTypeMap.erase(code);

    // Update remaining indices in map
    for (auto it = m_sessionTypeMap.begin(); it != m_sessionTypeMap.end(); ++it) {
        if (it->second > static_cast<size_t>(index)) {
            it->second--;
        }
    }

    return true;
}

bool TimetableManager::sessionTypeExists(const std::string& code) const {
    return m_sessionTypeMap.find(code) != m_sessionTypeMap.end();
}

// Room management
void TimetableManager::addRoom(const Room& room) {
    // Check if room already exists
    if (roomExists(room.getNumber())) {
        throw std::invalid_argument("Room with number " + room.getNumber() + " already exists");
    }

    // Add room to vector and map
    size_t index = m_rooms.size();
    m_rooms.push_back(room);
    m_roomMap[room.getNumber()] = index;
}

const std::vector<Room>& TimetableManager::getRooms() const {
    return m_rooms;
}

Room* TimetableManager::getRoom(const std::string& number) {
    auto it = m_roomMap.find(number);
    if (it != m_roomMap.end()) {
        return &m_rooms[it->second];
    }

    return nullptr;
}

Room& TimetableManager::getRoomAt(int index) {
    if (index < 0 || index >= static_cast<int>(m_rooms.size())) {
        throw std::out_of_range("Room index out of range");
    }

    return m_rooms[index];
}

bool TimetableManager::removeRoom(int index) {
    if (index < 0 || index >= static_cast<int>(m_rooms.size())) {
        return false;
    }

    // Get the room number before removing
    std::string number = m_rooms[index].getNumber();

    // Remove room from vector
    m_rooms.erase(m_rooms.begin() + index);

    // Remove from map
    m_roomMap.erase(number);

    // Update remaining indices in map
    for (auto it = m_roomMap.begin(); it != m_roomMap.end(); ++it) {
        if (it->second > static_cast<size_t>(index)) {
            it->second--;
        }
    }

    return true;
}

bool TimetableManager::roomExists(const std::string& number) const {
    return m_roomMap.find(number) != m_roomMap.end();
}

// Student management
void TimetableManager::addStudent(std::shared_ptr<Student> student) {
    // Check if student ID already exists
    if (studentExists(student->getStudentId())) {
        throw std::invalid_argument("Student with ID " + student->getStudentId() + " already exists");
    }

    // Check if username already exists
    if (studentUsernameExists(student->getUsername())) {
        throw std::invalid_argument("Student with username " + student->getUsername() + " already exists");
    }

    // Add student to vectors and maps
    m_students.push_back(student);
    m_studentMap[student->getStudentId()] = student;
    m_studentUsernameMap[student->getUsername()] = student;
    m_userMap[student->getUsername()] = student;
}

const std::vector<std::shared_ptr<Student>>& TimetableManager::getStudents() const {
    return m_students;
}

std::shared_ptr<Student> TimetableManager::getStudent(const std::string& studentId) const {
    auto it = m_studentMap.find(studentId);
    if (it != m_studentMap.end()) {
        return it->second;
    }

    return nullptr;
}

std::shared_ptr<Student> TimetableManager::getStudentByUsername(const std::string& username) const {
    auto it = m_studentUsernameMap.find(username);
    if (it != m_studentUsernameMap.end()) {
        return it->second;
    }

    return nullptr;
}

std::shared_ptr<Student> TimetableManager::getStudentAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_students.size())) {
        throw std::out_of_range("Student index out of range");
    }

    return m_students[index];
}

bool TimetableManager::removeStudent(const std::string& studentId) {
    auto it = m_studentMap.find(studentId);
    if (it == m_studentMap.end()) {
        return false;
    }

    std::shared_ptr<Student> student = it->second;

    // If student is assigned to a group, remove them
    if (student->getAssignedGroup()) {
        student->getAssignedGroup()->removeStudent(studentId);
    }

    // Remove from vectors and maps
    auto username = student->getUsername();
    m_students.erase(std::remove(m_students.begin(), m_students.end(), student), m_students.end());
    m_studentMap.erase(studentId);
    m_studentUsernameMap.erase(username);
    m_userMap.erase(username);

    return true;
}

bool TimetableManager::studentExists(const std::string& studentId) const {
    return m_studentMap.find(studentId) != m_studentMap.end();
}

bool TimetableManager::studentUsernameExists(const std::string& username) const {
    return m_studentUsernameMap.find(username) != m_studentUsernameMap.end();
}

bool TimetableManager::assignStudentToGroup(const std::string& studentId, const std::string& groupCode) {
    // Find the student
    auto studentIt = m_studentMap.find(studentId);
    if (studentIt == m_studentMap.end()) {
        return false;
    }

    // Find the group
    auto groupIt = m_groupMap.find(groupCode);
    if (groupIt == m_groupMap.end()) {
        return false;
    }

    // Get the student and group
    std::shared_ptr<Student> student = studentIt->second;
    Group* group = &m_groups[groupIt->second];

    // Remove student from current group if assigned
    if (student->getAssignedGroup()) {
        student->getAssignedGroup()->removeStudent(studentId);
    }

    // Add student to new group
    group->addStudent(student);

    return true;
}

// Lecturer management
void TimetableManager::addLecturer(std::shared_ptr<Lecturer> lecturer) {
    // Check if lecturer ID already exists
    if (lecturerExists(lecturer->getId())) {
        throw std::invalid_argument("Lecturer with ID " + lecturer->getId() + " already exists");
    }

    // Add lecturer to vector and map
    m_lecturers.push_back(lecturer);
    m_lecturerMap[lecturer->getId()] = lecturer;
}

const std::vector<std::shared_ptr<Lecturer>>& TimetableManager::getLecturers() const {
    return m_lecturers;
}

std::shared_ptr<Lecturer> TimetableManager::getLecturer(const std::string& lecturerId) const {
    auto it = m_lecturerMap.find(lecturerId);
    if (it != m_lecturerMap.end()) {
        return it->second;
    }

    return nullptr;
}

std::shared_ptr<Lecturer> TimetableManager::getLecturerAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_lecturers.size())) {
        throw std::out_of_range("Lecturer index out of range");
    }

    return m_lecturers[index];
}

bool TimetableManager::removeLecturer(const std::string& lecturerId) {
    auto it = m_lecturerMap.find(lecturerId);
    if (it == m_lecturerMap.end()) {
        return false;
    }

    std::shared_ptr<Lecturer> lecturer = it->second;

    // Remove from vector and map
    m_lecturers.erase(std::remove(m_lecturers.begin(), m_lecturers.end(), lecturer), m_lecturers.end());
    m_lecturerMap.erase(lecturerId);

    return true;
}

bool TimetableManager::lecturerExists(const std::string& lecturerId) const {
    return m_lecturerMap.find(lecturerId) != m_lecturerMap.end();
}

bool TimetableManager::assignLecturerToModule(const std::string& lecturerId, const std::string& moduleCode) {
    // Find the lecturer
    auto lecturerIt = m_lecturerMap.find(lecturerId);
    if (lecturerIt == m_lecturerMap.end()) {
        return false;
    }

    // Find the module
    auto moduleIt = m_moduleMap.find(moduleCode);
    if (moduleIt == m_moduleMap.end()) {
        return false;
    }

    // Get the lecturer and module
    std::shared_ptr<Lecturer> lecturer = lecturerIt->second;
    Module* module = &m_modules[moduleIt->second];

    // Add lecturer to module and module to lecturer
    lecturer->addModule(std::make_shared<Module>(*module));
    module->addLecturer(lecturer);

    return true;
}

// Admin management methods
void TimetableManager::addAdmin(std::shared_ptr<Admin> admin) {
    // Add admin to vectors and maps
    if (!admin) {
        throw std::invalid_argument("Admin pointer is null");
    }

    // Check if the username already exists in the user map
    if (m_userMap.find(admin->getUsername()) != m_userMap.end()) {
        throw std::invalid_argument("User with username " + admin->getUsername() + " already exists");
    }

    m_admins.push_back(admin);
    m_adminMap[admin->getUsername()] = admin;
    m_userMap[admin->getUsername()] = admin;
}

std::shared_ptr<Admin> TimetableManager::getAdminByUsername(const std::string& username) const {
    auto it = m_adminMap.find(username);
    if (it != m_adminMap.end()) {
        return it->second;
    }
    return nullptr;
}

// Session management
std::shared_ptr<Session> TimetableManager::createSession(
    const std::string& moduleCode,
    const std::string& sessionTypeCode,
    const std::string& roomNumber,
    const std::string& lecturerId,
    const TimeSlot& timeSlot,
    int weekNumber,
    const std::vector<std::string>& groupCodes) {

    // Check that all referenced entities exist
    Module* module = getModule(moduleCode);
    if (!module) {
        throw std::invalid_argument("Module with code " + moduleCode + " not found");
    }

    SessionType* sessionType = getSessionType(sessionTypeCode);
    if (!sessionType) {
        throw std::invalid_argument("Session type with code " + sessionTypeCode + " not found");
    }

    Room* room = getRoom(roomNumber);
    if (!room) {
        throw std::invalid_argument("Room with number " + roomNumber + " not found");
    }

    std::shared_ptr<Lecturer> lecturer = getLecturer(lecturerId);
    if (!lecturer) {
        throw std::invalid_argument("Lecturer with ID " + lecturerId + " not found");
    }

    // Validate week number
    if (weekNumber < 1 || weekNumber > 53) {
        throw std::invalid_argument("Week number must be between 1 and 53");
    }

    // Create the session
    std::string sessionId = generateSessionId();
    auto session = std::make_shared<Session>(
        sessionId,
        *module,
        *sessionType,
        *room,
        *lecturer,
        timeSlot,
        weekNumber
    );

    // Add groups to the session
    for (const auto& groupCode : groupCodes) {
        Group* group = getGroup(groupCode);
        if (!group) {
            throw std::invalid_argument("Group with code " + groupCode + " not found");
        }

        session->addGroup(std::make_shared<Group>(*group));
    }

    // Try to add the session to the timetable
    if (!m_timetable->addSession(session)) {
        throw std::runtime_error("Failed to add session due to conflicts");
    }

    // Add session to lecturer
    lecturer->addSession(session);

    return session;
}

std::vector<std::shared_ptr<Session>> TimetableManager::getAllSessions() const {
    return m_timetable->getAllSessions();
}

std::vector<std::shared_ptr<Session>> TimetableManager::getSessionsForGroup(
    const std::string& groupCode, int weekNumber) const {
    return m_timetable->getSessionsForWeekAndGroup(weekNumber, groupCode);
}

std::vector<std::shared_ptr<Session>> TimetableManager::getSessionsForModuleAndGroup(
    const std::string& moduleCode, const std::string& groupCode) const {
    return m_timetable->getSessionsForModuleAndGroup(moduleCode, groupCode);
}

std::vector<std::shared_ptr<Session>> TimetableManager::getSessionsForRoomAndGroup(
    const std::string& roomNumber, const std::string& groupCode) const {
    return m_timetable->getSessionsForRoomAndGroup(roomNumber, groupCode);
}

std::vector<std::shared_ptr<Session>> TimetableManager::getSessionsForLecturerAndGroup(
    const std::string& lecturerId, const std::string& groupCode) const {
    return m_timetable->getSessionsForLecturerAndGroup(lecturerId, groupCode);
}

std::vector<std::pair<std::shared_ptr<Session>, std::shared_ptr<Session>>> TimetableManager::findTimetableConflicts() const {
    return m_timetable->findConflicts();
}

std::shared_ptr<User> TimetableManager::authenticateUser(const std::string& username, const std::string& password) const {
    auto it = m_userMap.find(username);
    if (it != m_userMap.end()) {
        std::shared_ptr<User> user = it->second;
        if (user->verifyPassword(password)) {
            return user;
        }
    }

    return nullptr; // Authentication failed
}

std::string TimetableManager::generateSessionId() const {
    // Generate a random session ID
    static std::mt19937 gen(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::uniform_int_distribution<int> dist(10000, 99999);

    std::string id = "S" + std::to_string(dist(gen));

    // Make sure it's unique
    while (m_timetable->getSession(id) != nullptr) {
        id = "S" + std::to_string(dist(gen));
    }

    return id;
}