/**
 * @file FileManager.cpp
 * @brief Implementation of the FileManager class
 */

#include "FileManager.h"
#include "TimetableManager.h"
#include "Module.h"
#include "Group.h"
#include "SessionType.h"
#include "Room.h"
#include "Student.h"
#include "Lecturer.h"
#include "Session.h"
#include "TimeSlot.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <sys/stat.h>  // For checking if files exist
#include <direct.h>    // For _mkdir on Windows

// Constructor
FileManager::FileManager(const std::string& directory)
    : m_directory(directory) {
    Utils::createDirectoryIfNotExists(m_directory);
}

// Destructor
FileManager::~FileManager() {
    // Nothing to clean up
}

// Save all data to files
bool FileManager::saveData(const TimetableManager& manager) {
    bool success = true;
    success &= saveModules(manager);
    success &= saveGroups(manager);
    success &= saveSessionTypes(manager);
    success &= saveRooms(manager);
    success &= saveStudents(manager);
    success &= saveLecturers(manager);
    success &= saveSessions(manager);
    return success;
}

// Load all data from files
bool FileManager::loadData(TimetableManager& manager) {
    bool success = true;
    success &= loadModules(manager);
    success &= loadGroups(manager);
    success &= loadSessionTypes(manager);
    success &= loadRooms(manager);
    success &= loadStudents(manager);
    success &= loadLecturers(manager);
    success &= loadSessions(manager);
    return success;
}

// ---------------------- Saving Functions ----------------------

// Save modules to file
bool FileManager::saveModules(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "modules.csv")) return false;
    file << "Code,Name\n";
    for (const auto& module : manager.getModules())
        file << module.getCode() << "," << module.getName() << "\n";
    file.close();
    return true;
}

// Save groups to file
bool FileManager::saveGroups(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "groups.csv")) return false;
    file << "Code,Name\n";
    for (const auto& group : manager.getGroups())
        file << group.getCode() << "," << group.getName() << "\n";
    file.close();
    return true;
}

// Save session types to file
bool FileManager::saveSessionTypes(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "session_types.csv")) return false;
    file << "Code,Name,Description\n";
    for (const auto& sessionType : manager.getSessionTypes())
        file << sessionType.getCode() << "," 
             << sessionType.getName() << "," 
             << sessionType.getDescription() << "\n";
    file.close();
    return true;
}

// Save rooms to file
bool FileManager::saveRooms(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "rooms.csv")) return false;
    file << "Number,Name,Capacity,Type,Building\n";
    for (const auto& room : manager.getRooms())
        file << room.getNumber() << "," 
             << room.getName() << "," 
             << room.getCapacity() << "," 
             << static_cast<int>(room.getType()) << "," 
             << room.getBuilding() << "\n";
    file.close();
    return true;
}

// Save students to file
bool FileManager::saveStudents(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "students.csv")) return false;
    file << "Username,Password,FirstName,LastName,StudentID,GroupCode\n";
    for (const auto& student : manager.getStudents()) {
        file << student->getUsername() << "," 
             << student->getPassword() << "," 
             << student->getFirstName() << "," 
             << student->getLastName() << "," 
             << student->getStudentId() << ",";
        if (student->getAssignedGroup())
            file << student->getAssignedGroup()->getCode();
        file << "\n";
    }
    file.close();
    return true;
}

// Save lecturers to file
bool FileManager::saveLecturers(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "lecturers.csv")) return false;
    file << "ID,FirstName,LastName,Title,Department\n";
    for (const auto& lecturer : manager.getLecturers())
        file << lecturer->getId() << "," 
             << lecturer->getFirstName() << "," 
             << lecturer->getLastName() << "," 
             << lecturer->getTitle() << "," 
             << lecturer->getDepartment() << "\n";
    file.close();

    // Save lecturer-module assignments
    std::ofstream moduleFile;
    if (!openFileForWriting(moduleFile, "lecturer_modules.csv")) return false;
    moduleFile << "LecturerID,ModuleCode\n";
    for (const auto& lecturer : manager.getLecturers())
        for (const auto& module : lecturer->getModules())
            moduleFile << lecturer->getId() << "," << module->getCode() << "\n";
    moduleFile.close();
    return true;
}

// Save sessions to file
bool FileManager::saveSessions(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "sessions.csv")) return false;
    file << "ID,ModuleCode,SessionTypeCode,RoomNumber,LecturerID,Day,StartHour,StartMinute,EndHour,EndMinute,WeekNumber\n";
    for (const auto& session : manager.getAllSessions()) {
        const TimeSlot& timeSlot = session->getTimeSlot();
        file << session->getId() << "," 
             << session->getModule().getCode() << "," 
             << session->getSessionType().getCode() << "," 
             << session->getRoom().getNumber() << "," 
             << session->getLecturer().getId() << "," 
             << static_cast<int>(timeSlot.getDay()) << "," 
             << timeSlot.getStartTime() / 60 << "," 
             << timeSlot.getStartTime() % 60 << "," 
             << timeSlot.getEndTime() / 60 << "," 
             << timeSlot.getEndTime() % 60 << "," 
             << session->getWeekNumber() << "\n";
    }
    file.close();

    // Save session-group assignments
    std::ofstream groupFile;
    if (!openFileForWriting(groupFile, "session_groups.csv")) return false;
    groupFile << "SessionID,GroupCode\n";
    for (const auto& session : manager.getAllSessions())
        for (const auto& group : session->getGroups())
            groupFile << session->getId() << "," << group->getCode() << "\n";
    groupFile.close();
    return true;
}

// ---------------------- Loading Functions ----------------------

// Load modules from file
bool FileManager::loadModules(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "modules.csv")) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string code, name;
        std::getline(ss, code, ',');
        std::getline(ss, name, ',');
        manager.addModule(Module(code, name));
    }
    file.close();
    return true;
}

// Load groups from file
bool FileManager::loadGroups(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "groups.csv")) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string code, name;
        std::getline(ss, code, ',');
        std::getline(ss, name, ',');
        manager.addGroup(Group(code, name));
    }
    file.close();
    return true;
}

// Load session types from file
bool FileManager::loadSessionTypes(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "session_types.csv")) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string code, name, description;
        std::getline(ss, code, ',');
        std::getline(ss, name, ',');
        std::getline(ss, description, ',');
        manager.addSessionType(SessionType(code, name, description));
    }
    file.close();
    return true;
}

// Load rooms from file
bool FileManager::loadRooms(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "rooms.csv")) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string number, name, capacityStr, typeStr, building;
        std::getline(ss, number, ',');
        std::getline(ss, name, ',');
        std::getline(ss, capacityStr, ',');
        std::getline(ss, typeStr, ',');
        std::getline(ss, building, ',');
        int capacity = std::stoi(capacityStr);
        Room::RoomType type = static_cast<Room::RoomType>(std::stoi(typeStr));
        manager.addRoom(Room(number, name, capacity, type, building));
    }
    file.close();
    return true;
}

// Load students from file
bool FileManager::loadStudents(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "students.csv")) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password, firstName, lastName, studentId, groupCode;
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, firstName, ',');
        std::getline(ss, lastName, ',');
        std::getline(ss, studentId, ',');
        std::getline(ss, groupCode, ',');
        auto student = std::make_shared<Student>(username, password, firstName, lastName, studentId);
        manager.addStudent(student);
        if (!groupCode.empty())
            manager.assignStudentToGroup(studentId, groupCode);
    }
    file.close();
    return true;
}

// Load lecturers from file
bool FileManager::loadLecturers(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "lecturers.csv")) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, firstName, lastName, title, department;
        std::getline(ss, id, ',');
        std::getline(ss, firstName, ',');
        std::getline(ss, lastName, ',');
        std::getline(ss, title, ',');
        std::getline(ss, department, ',');
        manager.addLecturer(std::make_shared<Lecturer>(firstName, lastName, id, title, department));
    }
    // Load lecturer-module assignments
    std::ifstream moduleFile;
    if (openFileForReading(moduleFile, "lecturer_modules.csv")) {
        std::getline(moduleFile, line);
        while (std::getline(moduleFile, line)) {
            std::stringstream ss(line);
            std::string lecturerId, moduleCode;
            std::getline(ss, lecturerId, ',');
            std::getline(ss, moduleCode, ',');
            manager.assignLecturerToModule(lecturerId, moduleCode);
        }
        moduleFile.close();
    }
    file.close();
    return true;
}

// Load sessions from file
bool FileManager::loadSessions(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "sessions.csv")) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, moduleCode, sessionTypeCode, roomNumber, lecturerId;
        std::string dayStr, startHourStr, startMinuteStr, endHourStr, endMinuteStr, weekNumberStr;
        std::getline(ss, id, ',');
        std::getline(ss, moduleCode, ',');
        std::getline(ss, sessionTypeCode, ',');
        std::getline(ss, roomNumber, ',');
        std::getline(ss, lecturerId, ',');
        std::getline(ss, dayStr, ',');
        std::getline(ss, startHourStr, ',');
        std::getline(ss, startMinuteStr, ',');
        std::getline(ss, endHourStr, ',');
        std::getline(ss, endMinuteStr, ',');
        std::getline(ss, weekNumberStr, ',');
        int day = std::stoi(dayStr);
        int startHour = std::stoi(startHourStr);
        int startMinute = std::stoi(startMinuteStr);
        int endHour = std::stoi(endHourStr);
        int endMinute = std::stoi(endMinuteStr);
        int weekNumber = std::stoi(weekNumberStr);
        TimeSlot timeSlot(static_cast<TimeSlot::Day>(day), startHour, startMinute, endHour, endMinute);
        manager.createSession(moduleCode, sessionTypeCode, roomNumber, lecturerId, timeSlot, weekNumber);
    }
    file.close();

    // Load session-group assignments
    std::ifstream groupFile;
    if (openFileForReading(groupFile, "session_groups.csv")) {
        std::getline(groupFile, line);
        while (std::getline(groupFile, line)) {
            std::stringstream ss(line);
            std::string sessionId, groupCode;
            std::getline(ss, sessionId, ',');
            std::getline(ss, groupCode, ',');
            // Not implemented: manager.assignGroupToSession(sessionId, groupCode);
        }
        groupFile.close();
    }
    return true;
}

// ---------------------- Helper Functions ----------------------

// Open a file for writing
bool FileManager::openFileForWriting(std::ofstream& file, const std::string& filename) {
    std::string filepath = getFilePath(filename);
    file.open(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filepath << std::endl;
        return false;
    }
    return true;
}

// Open a file for reading
bool FileManager::openFileForReading(std::ifstream& file, const std::string& filename) {
    std::string filepath = getFilePath(filename);
    struct stat buffer;
    if (stat(filepath.c_str(), &buffer) != 0) return false;
    file.open(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filepath << std::endl;
        return false;
    }
    return true;
}

// Get the full file path
std::string FileManager::getFilePath(const std::string& filename) {
    return m_directory + "/" + filename;
}
