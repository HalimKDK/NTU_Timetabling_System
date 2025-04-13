/**
 *
 * This file contains the declaration of the CSV_Exporter class, which
 * handles exporting timetable data to CSV files.
 */

#ifndef CSV_EXPORTER_H
#define CSV_EXPORTER_H

#include <string>
#include <vector>
#include <memory>

 // Forward declaration
class Session;

/**
 * The CSV_Exporter class provides functionality for exporting
 * timetable data to CSV files.
 */
class CSV_Exporter {
public:
    // Constructs a new CSV_Exporter object
    CSV_Exporter();

    // Destroys the CSV_Exporter object
    ~CSV_Exporter();

    /**
     * Exports a list of sessions to a CSV file.
     *
     * sessions   List of sessions to be exported.
     * filename   Name of the CSV file to create.
     * title      Optional title for the timetable (e.g., "Week 1").
     * groupCode  Optional group code associated with the timetable.
     *
     * Returns true if the export was successful, false otherwise.
     */
    bool exportTimetable(
        const std::vector<std::shared_ptr<Session>>& sessions,
        const std::string& filename,
        const std::string& title = "",
        const std::string& groupCode = "");

private:
    /**
     * Converts a session object into a properly formatted CSV row.
     *
     * session  The session to convert.
     *
     * Returns a string representing the session in CSV format.
     */
    std::string formatSessionForCSV(const std::shared_ptr<Session>& session);

    /**
     * Escapes any special characters in a string to make it safe for CSV.
     *
     * str  The string to process.
     *
     * Returns the escaped string.
     */
    std::string escapeCSV(const std::string& str);
};

#endif // CSV_EXPORTER_H
