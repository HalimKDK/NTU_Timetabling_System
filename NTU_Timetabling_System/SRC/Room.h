/**
 * Room.h
 *
 * This file defines the Room class, which represents physical spaces
 * like lecture halls, labs, or tutorial rooms in the timetabling system.
 */

#ifndef ROOM_H
#define ROOM_H

#include <string>

 /**
  * A Room represents a physical location where sessions take place.
  * It keeps track of details like the room number, name, capacity, type, and the building it’s in.
  */
class Room {
public:
    // List of possible room types
    enum class RoomType {
        LECTURE_HALL,
        LAB,
        TUTORIAL,
        MEETING_ROOM,
        OTHER
    };

    // Creates a new Room with the given details
    Room(const std::string& number, const std::string& name,
        int capacity = 0, RoomType type = RoomType::OTHER,
        const std::string& building = "");

    // Destroys the Room object
    ~Room();

    // Returns the room number (like "MAE 202")
    const std::string& getNumber() const;

    // Changes the room number
    void setNumber(const std::string& number);

    // Returns the room’s descriptive name (like "John Clare Lecture Theatre")
    const std::string& getName() const;

    // Changes the room’s name
    void setName(const std::string& name);

    // Returns how many people the room can hold
    int getCapacity() const;

    // Updates the room’s capacity
    void setCapacity(int capacity);

    // Returns the type of room (Lecture Hall, Lab, etc.)
    RoomType getType() const;

    // Updates the type of room
    void setType(RoomType type);

    // Returns the room type as a human-readable string
    std::string getTypeString() const;

    // Returns the name of the building where this room is located
    const std::string& getBuilding() const;

    // Updates the building name
    void setBuilding(const std::string& building);

    // Returns a full description of the room combining the building and room number
    std::string getFullName() const;

private:
    std::string m_number;     // Room number (like "MAE 202")
    std::string m_name;       // Descriptive name (like "John Clare Lecture Theatre")
    int m_capacity;           // Maximum number of people the room can hold
    RoomType m_type;          // Type of room
    std::string m_building;   // Building where the room is located
};

#endif // ROOM_H

