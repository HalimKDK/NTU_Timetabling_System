/**
 *Implementation of the CSV_Exporter class
 */

#include "CSV_Exporter.h"
#include "Session.h"
#include "Module.h"
#include "SessionType.h"
#include "Room.h"
#include "Lecturer.h"
#include "TimeSlot.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

 // Constructor
CSV_Exporter::CSV_Exporter() {
    // Nothing to initialize
}

// Destructor
CSV_Exporter::~CSV_Exporter() {
    // Nothing to clean up
}

// Export a timetable to a CSV file
bool CSV_Exporter::exportTimetable(
    const std::vector<std::shared_ptr<Session>>& sessions,
    const std::string& filename,
    const std::string& title,
    const std::string& groupCode) {

    // Open file for writing
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false;
    }

    // Write header information
    if (!title.empty()) {
        file << "# " << title << "\n";
    }

    if (!groupCode.empty()) {
        file << "# Group: " << groupCode << "\n";
    }

    file << "# Generated on: " << Utils::getCurrentDateTime() << "\n\n";

    // Write CSV header
    file << "Day,Start Time,End Time,Module Code,Module Name,Session Type,Room,Lecturer\n";

    // Sort sessions by day and time
    std::vector<std::shared_ptr<Session>> sortedSessions = sessions;
    std::sort(sortedSessions.begin(), sortedSessions.end(),
        [](const std::shared_ptr<Session>& a, const std::shared_ptr<Session>& b) {
            // Sort by day first
            if (a->getTimeSlot().getDay() != b->getTimeSlot().getDay()) {
                return a->getTimeSlot().getDay() < b->getTimeSlot().getDay();
            }

            // Then by start time
            return a->getTimeSlot().getStartTime() < b->getTimeSlot().getStartTime();
        });

    // Write sorted sessions
    for (const auto& session : sortedSessions) {
        file << formatSessionForCSV(session) << "\n";
    }

    file.close();
    return true;
}

// Format a session for CSV export
std::string CSV_Exporter::formatSessionForCSV(const std::shared_ptr<Session>& session) {
    std::stringstream ss;

    // Get the time slot
    const TimeSlot& timeSlot = session->getTimeSlot();

    // Format the session data
    ss << escapeCSV(Utils::dayToString(timeSlot.getDay())) << ","
        << escapeCSV(timeSlot.getStartTimeString()) << ","
        << escapeCSV(timeSlot.getEndTimeString()) << ","
        << escapeCSV(session->getModule().getCode()) << ","
        << escapeCSV(session->getModule().getName()) << ","
        << escapeCSV(session->getSessionType().getName()) << ","
        << escapeCSV(session->getRoom().getFullName()) << ","
        << escapeCSV(session->getLecturer().getFullName());

    return ss.str();
}

// Escape a string for CSV export
std::string CSV_Exporter::escapeCSV(const std::string& str) {
    // If the string contains commas, quotes, or newlines, wrap it in quotes
    if (str.find(',') != std::string::npos ||
        str.find('"') != std::string::npos ||
        str.find('\n') != std::string::npos) {

        // Replace double quotes with double double quotes
        std::string escaped = str;
        size_t pos = 0;
        while ((pos = escaped.find('"', pos)) != std::string::npos) {
            escaped.replace(pos, 1, "\"\"");
            pos += 2;
        }

        // Wrap in quotes
        return "\"" + escaped + "\"";
    }

    return str;
}