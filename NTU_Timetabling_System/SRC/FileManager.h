/**
 *
 * This file declares the FileManager class, which is responsible for handling
 * the saving and loading of timetabling system data to and from files.
 */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <fstream>

 // Forward declarations for related classes
class TimetableManager;
class Module;
class Group;
class SessionType;
class Room;
class Student;
class Lecturer;
class Session;

/**
 * The FileManager class is responsible for saving and loading all data
 * used by the timetabling system. It manages reading and writing to files
 * for modules, groups, session types, rooms, students, lecturers, and sessions.
 */
class FileManager {
public:
    /**
     * Creates a FileManager with a specified directory for storing data files.
     * If no directory is specified, it uses "data" by default.
     */
    FileManager(const std::string& directory = "data");

    /**
     * Destroys the FileManager object and cleans up if needed.
     */
    ~FileManager();

    /**
     * Saves all timetabling data to files in the configured directory.
     * Returns true if successful, false if an error occurs.
     */
    bool saveData(const TimetableManager& manager);

    /**
     * Loads all timetabling data from files in the configured directory.
     * Returns true if successful, false if an error occurs.
     */
    bool loadData(TimetableManager& manager);

private:
    std::string m_directory;  // Directory where data is saved and loaded

    // Functions to save different categories of data
    bool saveModules(const TimetableManager& manager);
    bool saveGroups(const TimetableManager& manager);
    bool saveSessionTypes(const TimetableManager& manager);
    bool saveRooms(const TimetableManager& manager);
    bool saveStudents(const TimetableManager& manager);
    bool saveLecturers(const TimetableManager& manager);
    bool saveSessions(const TimetableManager& manager);

    // Functions to load different categories of data
    bool loadModules(TimetableManager& manager);
    bool loadGroups(TimetableManager& manager);
    bool loadSessionTypes(TimetableManager& manager);
    bool loadRooms(TimetableManager& manager);
    bool loadStudents(TimetableManager& manager);
    bool loadLecturers(TimetableManager& manager);
    bool loadSessions(TimetableManager& manager);

    // Helper methods for file operations
    bool openFileForWriting(std::ofstream& file, const std::string& filename);
    bool openFileForReading(std::ifstream& file, const std::string& filename);
    std::string getFilePath(const std::string& filename);
};

#endif // FILE_MANAGER_H

