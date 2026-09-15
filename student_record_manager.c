#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "students.dat"
#define NAME_LENGTH 60
#define COURSE_LENGTH 60

typedef struct {
    int id;
    char name[NAME_LENGTH];
    char course[COURSE_LENGTH];
    float marks;
} Student;

static void trim_newline(char *text) {
    text[strcspn(text, "\n")] = '\0';
}

static void read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    trim_newline(buffer);
}

static int read_int(const char *prompt) {
    char input[32];
    int value;
    while (1) {
        read_line(prompt, input, sizeof(input));
        if (sscanf(input, "%d", &value) == 1) return value;
        printf("Please enter a valid number.\n");
    }
}

static float read_marks(void) {
    char input[32];
    float marks;
    while (1) {
        read_line("Marks (0-100): ", input, sizeof(input));
        if (sscanf(input, "%f", &marks) == 1 && marks >= 0 && marks <= 100) return marks;
        printf("Please enter marks from 0 to 100.\n");
    }
}

static int student_exists(int id) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    if (file == NULL) return 0;
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

static void add_student(void) {
    Student student;
    FILE *file;

    student.id = read_int("Student ID: ");
    if (student_exists(student.id)) {
        printf("A student with ID %d already exists.\n", student.id);
        return;
    }
    read_line("Name: ", student.name, sizeof(student.name));
    read_line("Course: ", student.course, sizeof(student.course));
    student.marks = read_marks();

    file = fopen(DATA_FILE, "ab");
    if (file == NULL) {
        perror("Could not open data file");
        return;
    }
    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);
    printf("Student added successfully.\n");
}

static void print_header(void) {
    printf("\n%-8s %-25s %-25s %-8s\n", "ID", "Name", "Course", "Marks");
    printf("---------------------------------------------------------------------\n");
}

static void print_student(const Student *student) {
    printf("%-8d %-25s %-25s %-8.2f\n", student->id, student->name, student->course, student->marks);
}

static void view_students(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    int count = 0;
    if (file == NULL) {
        printf("No records found. Add a student first.\n");
        return;
    }
    print_header();
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        print_student(&student);
        count++;
    }
    fclose(file);
    if (count == 0) printf("No records found.\n");
}

static void search_student(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    int id = read_int("Student ID to search: ");
    if (file == NULL) {
        printf("No records found.\n");
        return;
    }
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) {
            print_header();
            print_student(&student);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Student with ID %d was not found.\n", id);
}

static void update_student(void) {
    FILE *file = fopen(DATA_FILE, "r+b");
    Student student;
    int id = read_int("Student ID to update: ");
    if (file == NULL) {
        printf("No records found.\n");
        return;
    }
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) {
            read_line("New name: ", student.name, sizeof(student.name));
            read_line("New course: ", student.course, sizeof(student.course));
            student.marks = read_marks();
            fseek(file, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);
            fclose(file);
            printf("Student updated successfully.\n");
            return;
        }
    }
    fclose(file);
    printf("Student with ID %d was not found.\n", id);
}

static void delete_student(void) {
    FILE *source = fopen(DATA_FILE, "rb");
    FILE *temp;
    Student student;
    int id = read_int("Student ID to delete: ");
    int found = 0;
    if (source == NULL) {
        printf("No records found.\n");
        return;
    }
    temp = fopen("students.tmp", "wb");
    if (temp == NULL) {
        perror("Could not create temporary file");
        fclose(source);
        return;
    }
    while (fread(&student, sizeof(Student), 1, source) == 1) {
        if (student.id == id) {
            found = 1;
        } else {
            fwrite(&student, sizeof(Student), 1, temp);
        }
    }
    fclose(source);
    fclose(temp);
    if (found) {
        remove(DATA_FILE);
        rename("students.tmp", DATA_FILE);
        printf("Student deleted successfully.\n");
    } else {
        remove("students.tmp");
        printf("Student with ID %d was not found.\n", id);
    }
}

int main(void) {
    int choice;
    do {
        printf("\n=== Student Record Manager ===\n");
        printf("1. Add student\n2. View students\n3. Search student\n4. Update student\n5. Delete student\n0. Exit\n");
        choice = read_int("Choose an option: ");
        switch (choice) {
            case 1: add_student(); break;
            case 2: view_students(); break;
            case 3: search_student(); break;
            case 4: update_student(); break;
            case 5: delete_student(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Please choose an option from 0 to 5.\n");
        }
    } while (choice != 0);
    return 0;
}
