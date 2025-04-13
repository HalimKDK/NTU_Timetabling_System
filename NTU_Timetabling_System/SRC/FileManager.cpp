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
#include <filesystem>

// Constructor
FileManager::FileManager(const std::string& directory)
    : m_directory(directory) {
    // Create the directory if it doesn't exist
    Utils::createDirectoryIfNotExists(m_directory);
}

// Destructor
FileManager::~FileManager() {
    // Nothing to clean up
}

// Save all data to files
bool FileManager::saveData(const TimetableManager& manager) {
    bool success = true;
    
    // Save each type of data
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
    
    // Load each type of data
    success &= loadModules(manager);
    success &= loadGroups(manager);
    success &= loadSessionTypes(manager);
    success &= loadRooms(manager);
    success &= loadStudents(manager);
    success &= loadLecturers(manager);
    success &= loadSessions(manager);
    
    return success;
}

// Save modules to file
bool FileManager::saveModules(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "modules.csv")) {
        return false;
    }
    
    // Write header
    file << "Code,Name\n";
    
    // Write modules
    for (const auto& module : manager.getModules()) {
        file << module.getCode() << ","
             << module.getName() << "\n";
    }
    
    file.close();
    return true;
}

// Save groups to file
bool FileManager::saveGroups(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "groups.csv")) {
        return false;
    }
    
    // Write header
    file << "Code,Name\n";
    
    // Write groups
    for (const auto& group : manager.getGroups()) {
        file << group.getCode() << ","
             << group.getName() << "\n";
    }
    
    file.close();
    return true;
}

// Save session types to file
bool FileManager::saveSessionTypes(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "session_types.csv")) {
        return false;
    }
    
    // Write header
    file << "Code,Name,Description\n";
    
    // Write session types
    for (const auto& sessionType : manager.getSessionTypes()) {
        file << sessionType.getCode() << ","
             << sessionType.getName() << ","
             << sessionType.getDescription() << "\n";
    }
    
    file.close();
    return true;
}

// Save rooms to file
bool FileManager::saveRooms(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "rooms.csv")) {
        return false;
    }
    
    // Write header
    file << "Number,Name,Capacity,Type,Building\n";
    
    // Write rooms
    for (const auto& room : manager.getRooms()) {
        file << room.getNumber() << ","
             << room.getName() << ","
             << room.getCapacity() << ","
             << static_cast<int>(room.getType()) << ","
             << room.getBuilding() << "\n";
    }
    
    file.close();
    return true;
}

// Save students to file
bool FileManager::saveStudents(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "students.csv")) {
        return false;
    }
    
    // Write header
    file << "Username,Password,FirstName,LastName,StudentID,GroupCode\n";
    
    // Write students
    for (const auto& student : manager.getStudents()) {
        file << student->getUsername() << ","
             << student->getPassword() << ","
             << student->getFirstName() << ","
             << student->getLastName() << ","
             << student->getStudentId() << ",";
        
        // Add group code if assigned
        if (student->getAssignedGroup()) {
            file << student->getAssignedGroup()->getCode();
        }
        
        file << "\n";
    }
    
    file.close();
    return true;
}

// Save lecturers to file
bool FileManager::saveLecturers(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "lecturers.csv")) {
        return false;
    }
    
    // Write header
    file << "ID,FirstName,LastName,Title,Department\n";
    
    // Write lecturers
    for (const auto& lecturer : manager.getLecturers()) {
        file << lecturer->getId() << ","
             << lecturer->getFirstName() << ","
             << lecturer->getLastName() << ","
             << lecturer->getTitle() << ","
             << lecturer->getDepartment() << "\n";
    }
    
    file.close();
    
    // Save lecturer-module assignments
    std::ofstream moduleFile;
    if (!openFileForWriting(moduleFile, "lecturer_modules.csv")) {
        return false;
    }
    
    // Write header
    moduleFile << "LecturerID,ModuleCode\n";
    
    // Write lecturer-module assignments
    for (const auto& lecturer : manager.getLecturers()) {
        for (const auto& module : lecturer->getModules()) {
            moduleFile << lecturer->getId() << ","
                      << module->getCode() << "\n";
        }
    }
    
    moduleFile.close();
    return true;
}

// Save sessions to file
bool FileManager::saveSessions(const TimetableManager& manager) {
    std::ofstream file;
    if (!openFileForWriting(file, "sessions.csv")) {
        return false;
    }
    
    // Write header
    file << "ID,ModuleCode,SessionTypeCode,RoomNumber,LecturerID,Day,StartHour,StartMinute,EndHour,EndMinute,WeekNumber\n";
    
    // Write sessions
    for (const auto& session : manager.getAllSessions()) {
        const TimeSlot& timeSlot = session->getTimeSlot();
        int startHour = timeSlot.getStartTime() / 60;
        int startMinute = timeSlot.getStartTime() % 60;
        int endHour = timeSlot.getEndTime() / 60;
        int endMinute = timeSlot.getEndTime() % 60;
        
        file << session->getId() << ","
             << session->getModule().getCode() << ","
             << session->getSessionType().getCode() << ","
             << session->getRoom().getNumber() << ","
             << session->getLecturer().getId() << ","
             << static_cast<int>(timeSlot.getDay()) << ","
             << startHour << ","
             << startMinute << ","
             << endHour << ","
             << endMinute << ","
             << session->getWeekNumber() << "\n";
    }
    
    file.close();
    
    // Save session-group assignments
    std::ofstream groupFile;
    if (!openFileForWriting(groupFile, "session_groups.csv")) {
        return false;
    }
    
    // Write header
    groupFile << "SessionID,GroupCode\n";
    
    // Write session-group assignments
    for (const auto& session : manager.getAllSessions()) {
        for (const auto& group : session->getGroups()) {
            groupFile << session->getId() << ","
                     << group->getCode() << "\n";
        }
    }
    
    groupFile.close();
    return true;
}

// Load modules from file
bool FileManager::loadModules(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "modules.csv")) {
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    // Read modules
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string code, name;
        
        std::getline(ss, code, ',');
        std::getline(ss, name, ',');
        
        // Create and add module
        Module module(code, name);
        manager.addModule(module);
    }
    
    file.close();
    return true;
}

// Load groups from file
bool FileManager::loadGroups(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "groups.csv")) {
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    // Read groups
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string code, name;
        
        std::getline(ss, code, ',');
        std::getline(ss, name, ',');
        
        // Create and add group
        Group group(code, name);
        manager.addGroup(group);
    }
    
    file.close();
    return true;
}

// Load session types from file
bool FileManager::loadSessionTypes(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "session_types.csv")) {
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    // Read session types
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string code, name, description;
        
        std::getline(ss, code, ',');
        std::getline(ss, name, ',');
        std::getline(ss, description, ',');
        
        // Create and add session type
        SessionType sessionType(code, name, description);
        manager.addSessionType(sessionType);
    }
    
    file.close();
    return true;
}

// Load rooms from file
bool FileManager::loadRooms(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "rooms.csv")) {
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    // Read rooms
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string number, name, capacityStr, typeStr, building;
        
        std::getline(ss, number, ',');
        std::getline(ss, name, ',');
        std::getline(ss, capacityStr, ',');
        std::getline(ss, typeStr, ',');
        std::getline(ss, building, ',');
        
        // Parse capacity and type
        int capacity = std::stoi(capacityStr);
        Room::RoomType type = static_cast<Room::RoomType>(std::stoi(typeStr));
        
        // Create and add room
        Room room(number, name, capacity, type, building);
        manager.addRoom(room);
    }
    
    file.close();
    return true;
}

// Load students from file
bool FileManager::loadStudents(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "students.csv")) {
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    // Read students
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password, firstName, lastName, studentId, groupCode;
        
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, firstName, ',');
        std::getline(ss, lastName, ',');
        std::getline(ss, studentId, ',');
        std::getline(ss, groupCode, ',');
        
        // Create and add student
        auto student = std::make_shared<Student>(username, password, firstName, lastName, studentId);
        manager.addStudent(student);
        
        // Assign to group if applicable
        if (!groupCode.empty()) {
            manager.assignStudentToGroup(studentId, groupCode);
        }
    }
    
    file.close();
    return true;
}

// Load lecturers from file
bool FileManager::loadLecturers(TimetableManager& manager) {
    std::ifstream file;
    if (!openFileForReading(file, "lecturers.csv")) {
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    // Read lecturers
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, firstName, lastName, title, department;
        
        std::getline(ss, id, ',');
        std::getline(ss, firstName, ',');
        std::getline(ss, lastName, ',');
        std::getline(ss, title, ',');
        std::getline(ss, department, ',');
        
        // Create and add lecturer
        auto lecturer = std::make_shared<Lecturer>(firstName, lastName, id, title, department);
        manager.addLecturer(lecturer);
    }
    
    // Load lecturer-module assignments
    std::ifstream moduleFile;
    if (openFileForReading(moduleFile, "lecturer_modules.csv")) {
        // Skip header
        std::getline(moduleFile, line);
        
        // Read lecturer-module assignments
        while (std::getline(moduleFile, line)) {
            std::stringstream ss(line);
            std::string lecturerId, moduleCode;
            
            std::getline(ss, lecturerId, ',');
            std::getline(ss, moduleCode, ',');
            
            // Assign lecturer to module
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
    if (!openFileForReading(file, "sessions.csv")) {
        return false;
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    // Read sessions
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
        
        // Parse numeric values
        int day = std::stoi(dayStr);
        int startHour = std::stoi(startHourStr);
        int startMinute = std::stoi(startMinuteStr);
        int endHour = std::stoi(endHourStr);
        int endMinute = std::stoi(endMinuteStr);
        int weekNumber = std::stoi(weekNumberStr);
        
        // Create time slot
        TimeSlot timeSlot(static_cast<TimeSlot::Day>(day), startHour, startMinute, endHour, endMinute);
        
        // Create session
        // (We'll add groups later)
        manager.createSession(moduleCode, sessionTypeCode, roomNumber, lecturerId, timeSlot, weekNumber);
    }
    
    file.close();
    
    // Load session-group assignments
    std::ifstream groupFile;
    if (openFileForReading(groupFile, "session_groups.csv")) {
        // Skip header
        std::getline(groupFile, line);
        
        // Read session-group assignments
        while (std::getline(groupFile, line)) {
            std::stringstream ss(line);
            std::string sessionId, groupCode;
            
            std::getline(ss, sessionId, ',');
            std::getline(ss, groupCode, ',');
            
            // Assign group to session
            // This would require a method in TimetableManager to add a group to a session
            // which we haven't implemented yet
        }
        
        groupFile.close();
    }
    
    return true;
}

// Helper function to open a file for writing
bool FileManager::openFileForWriting(std::ofstream& file, const std::string& filename) {
    std::string filepath = getFilePath(filename);
    file.open(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filepath << std::endl;
        return false;
    }
    
    return true;
}

// Helper function to open a file for reading
bool FileManager::openFileForReading(std::ifstream& file, const std::string& filename) {
    std::string filepath = getFilePath(filename);
    
    // Check if file exists
    if (!std::filesystem::exists(filepath)) {
        // This is not an error for initial runs
        return false;
    }
    
    file.open(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filepath << std::endl;
        return false;
    }
    
    return true;
}

// Helper function to get the full file path
std::string FileManager::getFilePath(const std::string& filename) {
    return m_directory + "/" + filename;
}