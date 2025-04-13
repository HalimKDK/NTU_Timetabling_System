/**
 *
 * This file contains the implementation of utility functions
 * used throughout the timetabling system.
 */

#include "Utils.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <chrono>
#include <ctime>
#include <random>
#include <iomanip>     // For std::setw, std::setfill, etc.
#include <fstream>     // For file operations
#include <direct.h>    // For _mkdir on Windows
#include <sys/stat.h>  // For directory checking

namespace Utils {

    // Converts a day enum to its string representation
    std::string dayToString(TimeSlot::Day day) {
        switch (day) {
        case TimeSlot::Day::MONDAY: return "Monday";
        case TimeSlot::Day::TUESDAY: return "Tuesday";
        case TimeSlot::Day::WEDNESDAY: return "Wednesday";
        case TimeSlot::Day::THURSDAY: return "Thursday";
        case TimeSlot::Day::FRIDAY: return "Friday";
        case TimeSlot::Day::SATURDAY: return "Saturday";
        case TimeSlot::Day::SUNDAY: return "Sunday";
        default: return "Unknown";
        }
    }

    // Converts a string to its corresponding day enum
    TimeSlot::Day stringToDay(const std::string& dayString) {
        std::string day = toLower(dayString);

        if (day == "monday" || day == "mon") return TimeSlot::Day::MONDAY;
        if (day == "tuesday" || day == "tue") return TimeSlot::Day::TUESDAY;
        if (day == "wednesday" || day == "wed") return TimeSlot::Day::WEDNESDAY;
        if (day == "thursday" || day == "thu") return TimeSlot::Day::THURSDAY;
        if (day == "friday" || day == "fri") return TimeSlot::Day::FRIDAY;
        if (day == "saturday" || day == "sat") return TimeSlot::Day::SATURDAY;
        if (day == "sunday" || day == "sun") return TimeSlot::Day::SUNDAY;

        // Default to Monday if the string is invalid
        return TimeSlot::Day::MONDAY;
    }

    // Prompts the user for a valid integer within a given range
    int getValidInteger(int min, int max) {
        int value;
        bool valid = false;

        while (!valid) {
            std::cin >> value;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number: ";
            }
            else if (value < min || value > max) {
                std::cout << "Please enter a number between " << min << " and " << max << ": ";
            }
            else {
                valid = true;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        return value;
    }

    // Prompts the user for a non-empty string
    std::string getValidString(const std::string& prompt) {
        std::string value;
        bool valid = false;

        while (!valid) {
            std::cout << prompt;
            std::getline(std::cin, value);

            if (value.empty()) {
                std::cout << "Input cannot be empty. ";
            }
            else {
                valid = true;
            }
        }
        return value;
    }

    // Prompts the user for a valid time (hour and minute)
    void getValidTime(const std::string& hourPrompt, const std::string& minutePrompt, int& hour, int& minute) {
        std::cout << hourPrompt;
        hour = getValidInteger(0, 23);

        std::cout << minutePrompt;
        minute = getValidInteger(0, 59);
    }

    // Generates a unique ID string with a given prefix and numeric length
    std::string generateUniqueId(const std::string& prefix, int length) {
        static std::mt19937 gen(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

        int maxVal = 1;
        for (int i = 0; i < length; ++i) maxVal *= 10;
        maxVal -= 1;

        std::uniform_int_distribution<int> dist(1, maxVal);

        std::stringstream ss;
        ss << prefix << std::setw(length) << std::setfill('0') << dist(gen);

        return ss.str();
    }

    // Converts a string to lowercase
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    // Removes leading and trailing whitespace from a string
    std::string trim(const std::string& str) {
        auto start = std::find_if_not(str.begin(), str.end(),
            [](unsigned char c) { return std::isspace(c); });
        auto end = std::find_if_not(str.rbegin(), str.rend(),
            [](unsigned char c) { return std::isspace(c); }).base();

        return (start < end) ? std::string(start, end) : std::string();
    }

    // Returns true if the string starts with the given prefix
    bool startsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() &&
            str.compare(0, prefix.size(), prefix) == 0;
    }

    // Returns true if the string ends with the given suffix
    bool endsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() &&
            str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    // Splits a string into a vector of tokens, using a given delimiter character
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Returns the current date and time as a formatted string (YYYY-MM-DD HH:MM:SS)
    std::string getCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        std::tm timeinfo;

#if defined(_MSC_VER)
        localtime_s(&timeinfo, &in_time_t);
#else
        localtime_r(&in_time_t, &timeinfo);
#endif

        ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // Returns true if the specified directory exists
    bool directoryExists(const std::string& path) {
        struct stat info;
        if (stat(path.c_str(), &info) != 0) {
            return false;
        }
        return (info.st_mode & S_IFDIR) != 0;
    }

    // Creates a directory if it does not already exist
    bool createDirectoryIfNotExists(const std::string& path) {
        try {
            if (directoryExists(path)) {
                return true;
            }

#if defined(_WIN32)
            return _mkdir(path.c_str()) == 0;
#else
            return mkdir(path.c_str(), 0755) == 0;
#endif
        }
        catch (const std::exception& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
            return false;
        }
    }
}
