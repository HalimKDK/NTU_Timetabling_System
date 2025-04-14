/**
 * Utils.h
 *
 * This file defines a collection of utility functions used throughout
 * the timetabling system. These utilities cover tasks like string
 * manipulation, input validation, date-time formatting, and file handling.
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "TimeSlot.h"


 /**
  * A namespace containing various helper functions for the system.
  */
namespace Utils {

    // Converts a Day enum value to its string equivalent (like "Monday")
    std::string dayToString(TimeSlot::Day day);

    // Converts a string like "Monday" to its corresponding Day enum value
    TimeSlot::Day stringToDay(const std::string& dayString);

    // Gets a valid integer from the user within a given range (min to max)
    int getValidInteger(int min, int max);

    // Prompts the user for a non-empty string value
    std::string getValidString(const std::string& prompt);

    // Gets a valid time input from the user, storing hour and minute values
    void getValidTime(const std::string& hourPrompt, const std::string& minutePrompt,
        int& hour, int& minute);

    // Generates a unique ID string with a specified prefix and numeric part length
    std::string generateUniqueId(const std::string& prefix, int length = 5);

    // Converts a string to lowercase
    std::string toLower(const std::string& str);

    // Removes whitespace from the start and end of a string
    std::string trim(const std::string& str);

    // Checks if a string starts with a specific prefix
    bool startsWith(const std::string& str, const std::string& prefix);

    // Checks if a string ends with a specific suffix
    bool endsWith(const std::string& str, const std::string& suffix);

    // Splits a string into a list of parts using a given delimiter
    std::vector<std::string> split(const std::string& str, char delimiter);

    // Returns the current date and time as a formatted string (YYYY-MM-DD HH:MM:SS)
    std::string getCurrentDateTime();

    // Creates a directory if it doesn’t already exist
    bool createDirectoryIfNotExists(const std::string& path);
}

#endif // UTILS_H
