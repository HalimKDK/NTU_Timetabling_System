/**
 *
 * This file contains the implementation of the TimeSlot class,
 * which represents a time slot for a session in the timetabling system.
 */

#include "TimeSlot.h"
#include <iomanip>
#include <sstream>

 // Constructor to initialize the time slot with given day, start time, and end time
TimeSlot::TimeSlot(Day day, int startHour, int startMinute, int endHour, int endMinute)
    : m_day(day) {
    // Convert the start and end times to minutes since midnight
    m_startTime = timeToMinutes(startHour, startMinute);
    m_endTime = timeToMinutes(endHour, endMinute);

    // Ensure the end time is after the start time
    if (m_endTime <= m_startTime) {
        // If invalid, default to a 1-hour time slot
        m_endTime = m_startTime + 60;
    }
}

// Destructor
TimeSlot::~TimeSlot() {
    // No cleanup needed
}

// Get the day of the week for this time slot
TimeSlot::Day TimeSlot::getDay() const {
    return m_day;
}

// Set the day of the week for this time slot
void TimeSlot::setDay(Day day) {
    m_day = day;
}

// Get the start time in minutes since midnight
int TimeSlot::getStartTime() const {
    return m_startTime;
}

// Set the start time, adjusting the end time if needed
void TimeSlot::setStartTime(int hour, int minute) {
    int newStartTime = timeToMinutes(hour, minute);

    // Ensure the start time is before the end time
    if (newStartTime < m_endTime) {
        m_startTime = newStartTime;
    }
    else {
        // If the start time is invalid, adjust the end time to be 1 hour after the start time
        m_startTime = newStartTime;
        m_endTime = m_startTime + 60;
    }
}

// Get the end time in minutes since midnight
int TimeSlot::getEndTime() const {
    return m_endTime;
}

// Set the end time, ensuring it is after the start time
void TimeSlot::setEndTime(int hour, int minute) {
    int newEndTime = timeToMinutes(hour, minute);

    // Only update if the new end time is after the start time
    if (newEndTime > m_startTime) {
        m_endTime = newEndTime;
    }
    // If invalid, no changes are made
}

// Get the duration of the time slot in minutes
int TimeSlot::getDuration() const {
    return m_endTime - m_startTime;
}

// Check if this time slot overlaps with another time slot
bool TimeSlot::overlaps(const TimeSlot& other) const {
    // If the days are different, there is no overlap
    if (m_day != other.m_day) {
        return false;
    }

    // Check if there is an overlap in time
    return (m_startTime < other.m_endTime) && (m_endTime > other.m_startTime);
}

// Get the start time as a formatted string (HH:MM)
std::string TimeSlot::getStartTimeString() const {
    return minutesToString(m_startTime);
}

// Get the end time as a formatted string (HH:MM)
std::string TimeSlot::getEndTimeString() const {
    return minutesToString(m_endTime);
}

// Get the time slot as a string (formatted as Day HH:MM-HH:MM)
std::string TimeSlot::toString() const {
    std::string dayString;

    // Convert the day enum to a string
    switch (m_day) {
    case Day::MONDAY:    dayString = "Monday"; break;
    case Day::TUESDAY:   dayString = "Tuesday"; break;
    case Day::WEDNESDAY: dayString = "Wednesday"; break;
    case Day::THURSDAY:  dayString = "Thursday"; break;
    case Day::FRIDAY:    dayString = "Friday"; break;
    case Day::SATURDAY:  dayString = "Saturday"; break;
    case Day::SUNDAY:    dayString = "Sunday"; break;
    }

    return dayString + " " + getStartTimeString() + "-" + getEndTimeString();
}

// Convert hours and minutes to minutes since midnight
int TimeSlot::timeToMinutes(int hour, int minute) const {
    // Validate the hour and minute values
    hour = (hour >= 0 && hour < 24) ? hour : 0;
    minute = (minute >= 0 && minute < 60) ? minute : 0;

    // Return the total minutes since midnight
    return hour * 60 + minute;
}

// Convert minutes since midnight to a string (formatted as HH:MM)
std::string TimeSlot::minutesToString(int minutes) const {
    int hours = minutes / 60;
    int mins = minutes % 60;

    // Format the time as HH:MM
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << mins;

    return ss.str();
}
