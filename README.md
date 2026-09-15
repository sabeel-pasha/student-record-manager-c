# Student Record Manager (C)

A command-line application for managing student records. It stores records in a local binary file, so data remains available after the program closes.

## Features

- Add student records
- View all stored records in a formatted table
- Search by student ID
- Update a student's name, course, and marks
- Delete a student record
- Validate duplicate IDs and marks between 0 and 100

## Built with

- C
- Standard C library
- File handling with binary files

## Run locally

Compile the program with GCC:

```bash
gcc -Wall -Wextra -std=c11 student_record_manager.c -o student_record_manager
```

Run it:

```bash
./student_record_manager
```

On Windows, run `student_record_manager.exe`.

## Sample menu

```text
=== Student Record Manager ===
1. Add student
2. View students
3. Search student
4. Update student
5. Delete student
0. Exit
```

## What I learned

- Structures and functions in C
- Reading and writing binary files
- Input validation
- Building a menu-driven command-line application

## Future improvements

- Sort records by name or marks
- Add password protection for administrator access
- Export records to a CSV file
