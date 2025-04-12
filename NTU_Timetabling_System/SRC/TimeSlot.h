/**
 *
 * This file contains the declaration of the TimeSlot class,
 * which represents time slots for sessions in the timetabling system.
 */

#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <string>

 /**
  * Class representing a time slot for a session.
  *
  * The TimeSlot class represents a time slot for a session,
  * including the day, start time, and end time.
  */
class TimeSlot {
public:
    // Days of the week
    enum class Day {
        MONDAY = 0,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };

    // Constructor that initializes the time slot with default values
    TimeSlot(Day day = Day::MONDAY,
        int startHour = 9, int startMinute = 0,
        int endHour = 10, int endMinute = 0);

    // Destructor
    ~TimeSlot();

    // Get the day of the week for this time slot
    Day getDay() const;

    // Set the day of the week for this time slot
    void setDay(Day day);

    // Get the start time as minutes since midnight
    int getStartTime() const;

    // Set the start time using hour and minute values
    void setStartTime(int hour, int minute);

    // Get the end time as minutes since midnight
    int getEndTime() const;

    // Set the end time using hour and minute values
    void setEndTime(int hour, int minute);

    // Get the duration of the time slot in minutes
    int getDuration() const;

    // Check if this time slot overlaps with another time slot
    bool overlaps(const TimeSlot& other) const;

    // Get the start time as a string (formatted as HH:MM)
    std::string getStartTimeString() const;

    // Get the end time as a string (formatted as HH:MM)
    std::string getEndTimeString() const;

    // Get the time slot as a string (formatted as Day HH:MM-HH:MM)
    std::string toString() const;

private:
    Day m_day;         // Day of the week
    int m_startTime;   // Start time in minutes since midnight
    int m_endTime;     // End time in minutes since midnight

    // Helper method to convert hours and minutes to minutes since midnight
    int timeToMinutes(int hour, int minute) const;

    // Helper method to convert minutes since midnight to a string in HH:MM format
    std::string minutesToString(int minutes) const;
};

#endif // TIMESLOT_H

