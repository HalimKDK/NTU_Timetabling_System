# NTU_Timetabling_System

A C++ console application that simulates a simplified version of the Nottingham Trent University timetabling system. It was built for the SOFT20091 Software Design and Implementation module.

Admins build and maintain the academic timetable. Students log in to view, search and export their own timetable.

---

## What the project focuses on

- **Object-oriented design in C++**: inheritance, polymorphism, encapsulation and abstract base classes.
- **Use of the STL**: `std::vector`, `std::map`, `std::shared_ptr`, `std::unique_ptr` and `std::sort` for storage, lookup and sorting.
- **Separation of concerns**: user interface, business logic, data storage and utilities live in separate classes.
- **Robustness**: input validation, exception handling and conflict detection when scheduling sessions.

---

## Features

### Admin
- Add, edit, delete and list modules
- Add, edit, delete and list student groups
- Define and manage session types (Lecture, Laboratory, Tutorial, Seminar, Workshop are provided by default)
- Register students and assign them to groups
- Register lecturers and assign them to modules
- Add, edit, delete and list rooms (lecture halls, labs, and so on)
- Create timetable entries by week number (Weeks 1 to 53)
- Search for timetable conflicts such as double-booked rooms or lecturers (advanced feature)

### Student
- View the timetable for a chosen week, based on the student's assigned group
- Search the timetable by module, room or lecturer (advanced feature)
- Export a week's timetable to a CSV file (advanced feature)

---

## Getting started

### Requirements
- Windows with Visual Studio (2019 or later recommended)
- C++14 or later (the code uses `std::make_unique`)

### Build and run
1. Open the solution in Visual Studio.
2. Select the **Debug** or **Release** configuration.
3. Build with **Build > Rebuild Solution**.
4. Run with **Ctrl + F5**.

On the first run, if no saved data is found, the system creates sample data and saves it to the `data` folder.

### Sample logins

| Role    | Username | Password   |
|---------|----------|------------|
| Admin   | `admin`  | `admin123` |
| Student | `alice`  | `pass123`  |
| Student | `bob`    | `pass456`  |

Alice is in Group A and Bob is in Group B.

### Sample data
- **Modules:** SOFT20091, COMP20011, DATA20042
- **Groups:** A and B
- **Rooms:** MAE 202 (lab), JCLT 006 (lecture hall)
- **Lecturers:** Dr John Smith, Prof Sarah Jones
- **Sessions:** lectures and labs for Week 1

---

## How to use the system

### As an admin
1. Log in with the admin credentials.
2. Choose a management area from the main menu (modules, groups, session types, students, lecturers, rooms, timetable).
3. Follow the prompts to add, edit, delete or list records.
4. Under **Manage Timetable**, create entries by choosing a module, session type, room, lecturer, week, day, time and groups. Use **Find Timetable Conflicts** to check for clashes.

### As a student
1. Log in with your student credentials.
2. Choose **View Timetable** and enter a week number (1 to 53).
3. Choose **Search Timetable** to filter by module, room or lecturer.
4. Choose **Export Timetable to CSV** to save a week's timetable to a file.

---

## Architecture

```
main (NTU_Timetabling_System.cpp)
        |
        v
TimetableManager  (singleton, central coordinator)
        |
        +-- Modules, Groups, SessionTypes, Rooms
        +-- Students, Lecturers, Admins
        +-- Timetable (sessions and conflict detection)
        +-- FileManager (save and load)
```

### Class hierarchy

```
Person  <-- Student
        <-- Lecturer

User (abstract)  <-- Student
                 <-- Admin

UserInterface (abstract)  <-- Student
                          <-- Admin
```

| Class              | Responsibility                                                         |
|--------------------|------------------------------------------------------------------------|
| `TimetableManager` | Singleton that owns all data and exposes add, get, remove and search operations |
| `User`             | Abstract base for logins, with username, password and user type        |
| `UserInterface`    | Abstract base for console menus (header, footer, messages, input pauses) |
| `Person`           | Shared name and ID details for students and lecturers                  |
| `Admin`            | Admin menus and management operations                                  |
| `Student`          | Student menus, timetable viewing, searching and export                 |
| `Lecturer`         | Lecturer details, plus the modules and sessions they teach             |
| `Module`           | Module code, name and assigned lecturers                               |
| `Group`            | Student group and its members                                          |
| `Room`             | Room number, name, capacity, type and building                         |
| `SessionType`      | Type of session (lecture, lab, and so on)                              |
| `TimeSlot`         | Day, start time and end time, with overlap checking                    |
| `Session`          | A scheduled event linking module, type, room, lecturer, time and groups |
| `Timetable`        | Stores sessions, queries them and finds conflicts                      |
| `FileManager`      | Saves and loads system data                                            |
| `CSV_Exporter`     | Exports a timetable to CSV                                             |
| `Utils`            | Helpers for input validation and string handling                       |

### Design patterns and principles
- **Singleton**: `TimetableManager` provides one shared point of access to the data.
- **Polymorphism**: `Admin` and `Student` implement the same `User` and `UserInterface` methods, so the login flow treats them uniformly.
- **Smart pointers**: `std::shared_ptr` and `std::unique_ptr` manage object lifetimes.
- **Lookup maps**: `std::map` gives fast access by code or ID, alongside `std::vector` for ordered storage.

---

## Project structure

```
SRC/      Source code (.cpp and .h files)
TESTS/    Test source code and instructions for running the tests
BUILD/    Build files
data/     Saved data, created on first run
```

---

## Data validation and error handling

- Menu input is validated so that only numbers in the valid range are accepted.
- Week numbers must be between 1 and 53.
- Duplicate modules, groups, rooms, students, lecturers and usernames are rejected.
- Sessions that reference something that doesn't exist, or that clash with an existing session, are rejected with an exception message.

---

## Known limitations

- Updating and deleting existing timetable entries is not fully implemented yet.
- Passwords are stored in plain text, which is acceptable for a coursework prototype but not for real use.
- Timetable entries are created for one week at a time.

---

## Testing

See the `TESTS` folder for the test plan, unit tests and user acceptance test cases. Each test case lists an ID, scenario, steps, expected result, actual result and pass or fail.

---

## Author

**Name:** Halim Kadiku
