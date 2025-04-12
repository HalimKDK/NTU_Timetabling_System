/**
 *
 * This file contains the implementation of the Room class,
 * which models physical spaces used in a timetabling system.
 */

#include "Room.h"

 // Constructor that initializes all member variables
Room::Room(const std::string& number, const std::string& name,
    int capacity, RoomType type, const std::string& building)
    : m_number(number), m_name(name), m_capacity(capacity),
    m_type(type), m_building(building) {
    // No additional setup needed
}

// Destructor
Room::~Room() {
    // No manual cleanup required
}

// Returns the room number (e.g., "MAE 202")
const std::string& Room::getNumber() const {
    return m_number;
}

// Updates the room number
void Room::setNumber(const std::string& number) {
    m_number = number;
}

// Returns the room's descriptive name (e.g., "John Clare Lecture Theatre")
const std::string& Room::getName() const {
    return m_name;
}

// Updates the room name
void Room::setName(const std::string& name) {
    m_name = name;
}

// Returns how many people the room can accommodate
int Room::getCapacity() const {
    return m_capacity;
}

// Sets the room capacity, ensuring it's not negative
void Room::setCapacity(int capacity) {
    m_capacity = (capacity >= 0) ? capacity : 0;
}

// Returns the room's type (Lecture Hall, Lab, etc.)
Room::RoomType Room::getType() const {
    return m_type;
}

// Updates the type of room
void Room::setType(RoomType type) {
    m_type = type;
}

// Converts the room type to a human-readable string
std::string Room::getTypeString() const {
    switch (m_type) {
    case RoomType::LECTURE_HALL:
        return "Lecture Hall";
    case RoomType::LAB:
        return "Laboratory";
    case RoomType::TUTORIAL:
        return "Tutorial Room";
    case RoomType::MEETING_ROOM:
        return "Meeting Room";
    case RoomType::OTHER:
    default:
        return "Other";
    }
}

// Returns the name of the building where this room is located
const std::string& Room::getBuilding() const {
    return m_building;
}

// Updates the building name
void Room::setBuilding(const std::string& building) {
    m_building = building;
}

// Returns a formatted full name, combining building and room number
std::string Room::getFullName() const {
    if (m_building.empty()) {
        return m_number;
    }

    return m_building + " " + m_number;
}
