#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../../../libc/cstring.h"
#include "../../../libc/errors.h"
#include "../../../libc/input.h"
#include "../../../libc/memory.h"
#include "../../../libc/utils.h"

#define GRADES_COUNT (5)
#define INITIAL_CAPACITY (16)
#define GROWTH_FACTOR (2)

// Optinos fron user obtained by CLI arguments or through interattive menu
typedef struct {
    FILE *input_file;   // -f
    FILE *output_file;  // -o
} CLIOptions;

typedef struct Student {
    size_t id;
    String name;
    String last_name;
    String group;
    unsigned char *grades;
} Student;

typedef struct Students_data {
    Student *students;
    double average_grade;
    size_t size;
    size_t capacity;
} Students_data;

// Utils
err_t time_to_number(size_t *number, String date);

// Memory managment
err_t read_students_from_file(FILE *fin, Students_data *s);
void students_free(Students_data *s);

// CLI options
err_t parse_cli_arguments(const int argc, const char *argv[],
                          CLIOptions *options);

// User interface
void print_students_menu();

// Get student data
err_t student_print_by_id(const FILE *fout, Students_data *s, size_t id);
err_t student_print_by_last_name(const FILE *fout, Students_data *s,
                                 String last_name);
err_t student_print_by_name(const FILE *fout, Students_data *s, String name);
err_t student_print_by_group(const FILE *fout, Students_data *s, String group);

// Sort
void students_sort(Students_data *s,
                   int (*__comparer)(const void *, const void *));

int compare_by_id(const void *a, const void *b);
int compare_by_last_name(const void *a, const void *b);
int compare_by_name(const void *a, const void *b);
int compare_by_group(const void *a, const void *b);

// Output
err_t student_print(const FILE *fout, const Student *s);
err_t print_best_students(const FILE *fout, const Students_data *s);

err_t program_04_4(int argc, char *argv[]) {
    double grade;
    String s_ans = NULL;
    char c;
    size_t id;
    int toexit = 0;
    unsigned char ans;
    err_t err;
    CLIOptions options;
    Students_data s;

    if (argc < 3) {
        return INVALID_FLAG;
    }

    err = parse_cli_arguments(argc, (const char **)argv, &options);
    if (err) {
        fclose(options.input_file);
        if (options.output_file != stdout) {
            fclose(options.output_file);
        }
        return err;
    }
    err = read_students_from_file(options.input_file, &s);
    if (err) {
        fclose(options.input_file);
        fclose(options.output_file);
        students_free(&s);
        return err;
    }

    while (1) {
        clear_screen();
        print_students_menu();
        fflush(stdin);
        scanf("%hhu", &ans);
        switch (ans) {
            case 0:
                toexit = 1;
                break;

            case 1:
                printf("Enter student id: ");
                scanf("%zu", &id);
                err = student_print_by_id(stdout, &s, id);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    students_free(&s);
                    return err;
                }
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 2:
                printf("Enter student last name: ");
                while ((c = getchar()) != '\n' && c != EOF);
                err = read_string_from_user(&s_ans);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    students_free(&s);
                    string_free(s_ans);
                    return err;
                }
                err = student_print_by_last_name(stdout, &s, s_ans);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    string_free(s_ans);
                    students_free(&s);
                    return err;
                }
                string_free(s_ans);
                s_ans = NULL;
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 3:
                printf("Enter student name: ");
                while ((c = getchar()) != '\n' && c != EOF);
                err = read_string_from_user(&s_ans);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    students_free(&s);
                    string_free(s_ans);
                    return err;
                }
                err = student_print_by_name(stdout, &s, s_ans);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    string_free(s_ans);
                    students_free(&s);
                    return err;
                }
                string_free(s_ans);
                s_ans = NULL;
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 4:
                printf("Enter student group: ");
                while ((c = getchar()) != '\n' && c != EOF);
                err = read_string_from_user(&s_ans);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    students_free(&s);
                    string_free(s_ans);
                    return err;
                }
                err = student_print_by_group(stdout, &s, s_ans);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    string_free(s_ans);
                    students_free(&s);
                    return err;
                }
                string_free(s_ans);
                s_ans = NULL;
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 5:
                students_sort(&s, compare_by_id);
                printf("\nDone.\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 6:
                students_sort(&s, compare_by_last_name);
                printf("\nDone.\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 7:
                students_sort(&s, compare_by_name);
                printf("\nDone.\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 8:
                students_sort(&s, compare_by_group);
                printf("\nDone.\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;
            case 9:
                if (options.output_file == stdout) {
                    printf(
                        "You didn't specify output file. Printing to "
                        "stdout.\n");
                }
                printf("Enter student id: ");
                scanf("%zu", &id);
                err = student_print_by_id(options.output_file, &s, id);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    students_free(&s);
                    return err;
                }
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 10:
                if (options.output_file == stdout) {
                    printf(
                        "You didn't specify output file. Printing to "
                        "stdout.\n");
                }
                err = print_best_students(options.output_file, &s);
                if (err) {
                    fclose(options.input_file);
                    fclose(options.output_file);
                    students_free(&s);
                    return err;
                }
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;
        }
        if (toexit) {
            break;
        }
    }

    students_free(&s);
    return EXIT_SUCCESS;
}

err_t read_students_from_file(FILE *fin, Students_data *s) {
    if (fin == NULL || s == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    size_t i;
    err_t err;
    unsigned char grades[GRADES_COUNT];
    size_t id_buf;
    char last_name_buf[BUFSIZ];
    char name_buf[BUFSIZ];
    char group_buf[BUFSIZ];

    if (is_file_empty(fin)) {
        fclose(fin);
        s->students = NULL;
        return EXIT_SUCCESS;
    }

    s->students = (Student *)malloc(sizeof(Student) * INITIAL_CAPACITY);
    if (s->students == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    s->average_grade = 0;
    s->size = 0;
    s->capacity = INITIAL_CAPACITY;

    while (fscanf(fin, "%zu %s %s %s %hhu %hhu %hhu %hhu %hhu", &id_buf,
                  last_name_buf, name_buf, group_buf, &grades[0], &grades[1],
                  &grades[2], &grades[3], &grades[4]) == 9) {
        for (i = 0; i < GRADES_COUNT; ++i) {
            s->average_grade += grades[i];
        }
        if (s->size >= s->capacity) {
            err = rerealloc((void **)&(s->students),
                            sizeof(Student) * s->capacity * GROWTH_FACTOR);
            if (err) {
                students_free(s);
                return err;
            }
            s->capacity *= GROWTH_FACTOR;
        }

        Student *current_student = &s->students[s->size];
        current_student->id = id_buf;
        current_student->last_name = string_from(last_name_buf);
        current_student->name = string_from(name_buf);
        current_student->group = string_from(group_buf);
        current_student->grades =
            (unsigned char *)malloc(sizeof(unsigned char) * GRADES_COUNT);

        if (current_student->last_name == NULL ||
            current_student->name == NULL || current_student->group == NULL ||
            current_student->grades == NULL) {
            students_free(s);
            return MEMORY_ALLOCATION_ERROR;
        }

        memcpy(current_student->grades, grades,
               GRADES_COUNT * sizeof(unsigned char));
        s->size++;
    }

    if (s->size > 0) {
        s->average_grade /= (s->size * GRADES_COUNT);
    } else {
        s->average_grade = 0;
    }

    if (s->size < s->capacity) {
        err = rerealloc((void **)&(s->students), s->size * sizeof(Student));
        if (err) {
            students_free(s);
            return err;
        }
    }
    return EXIT_SUCCESS;
}

void students_free(Students_data *s) {
    size_t i;
    if (s == NULL) {
        return;
    }
    for (i = 0; i < s->size; ++i) {
        string_free(s->students[i].last_name);
        string_free(s->students[i].name);
        string_free(s->students[i].group);
        free(s->students[i].grades);
    }
    free(s->students);
}

err_t parse_cli_arguments(const int argc, const char *argv[],
                          CLIOptions *options) {
    int i;

    if (options == NULL || argv == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    memset((void *)options, 0, sizeof(CLIOptions));
    options->output_file = stdout;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            options->input_file = fopen(argv[++i], "r");
            if (options->input_file == NULL) {
                return OPENING_THE_FILE_ERROR;
            }
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            options->output_file = fopen(argv[++i], "w");
            if (options->input_file == NULL) {
                return OPENING_THE_FILE_ERROR;
            }
        } else {
            return INVALID_FLAG;
        }
    }
    return EXIT_SUCCESS;
}

err_t student_print_by_id(const FILE *fout, Students_data *s, size_t id) {
    size_t i;

    if (fout == NULL || s == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    for (i = 0; i < s->size; ++i) {
        if (s->students[i].id == id) {
            student_print(fout, s->students + i);
        }
    }
    return EXIT_SUCCESS;
}

err_t student_print_by_last_name(const FILE *fout, Students_data *s,
                                 String last_name) {
    size_t i;

    if (fout == NULL || s == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    for (i = 0; i < s->size; ++i) {
        if (!string_cmp(s->students[i].last_name, last_name)) {
            student_print(fout, s->students + i);
        }
    }
    return EXIT_SUCCESS;
}

err_t student_print_by_name(const FILE *fout, Students_data *s, String name) {
    size_t i;

    if (fout == NULL || s == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    for (i = 0; i < s->size; ++i) {
        if (!string_cmp(s->students[i].name, name)) {
            student_print(fout, s->students + i);
        }
    }
    return EXIT_SUCCESS;
}

err_t student_print_by_group(const FILE *fout, Students_data *s, String group) {
    size_t i;

    if (fout == NULL || s == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    for (i = 0; i < s->size; ++i) {
        if (!string_cmp(s->students[i].group, group)) {
            student_print(fout, s->students + i);
        }
    }
    return EXIT_SUCCESS;
}

void students_sort(Students_data *s,
                   int (*__comparer)(const void *, const void *)) {
    qsort((void *)s->students, s->size, sizeof(Student), __comparer);
}

int compare_by_id(const void *a, const void *b) {
    Student *ap = (Student *)a;
    Student *bp = (Student *)b;
    return ap->id - bp->id;
}

int compare_by_last_name(const void *a, const void *b) {
    Student *ap = (Student *)a;
    Student *bp = (Student *)b;
    return string_cmp(ap->last_name, bp->last_name);
}

int compare_by_name(const void *a, const void *b) {
    Student *ap = (Student *)a;
    Student *bp = (Student *)b;
    return string_cmp(ap->name, bp->name);
}

int compare_by_group(const void *a, const void *b) {
    Student *ap = (Student *)a;
    Student *bp = (Student *)b;
    return string_cmp(ap->group, bp->group);
}

err_t student_print(const FILE *fout, const Student *s) {
    double avg_grade = 0;
    size_t i;

    if (fout == NULL || s == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    for (i = 0; i < GRADES_COUNT; ++i) {
        avg_grade += s->grades[i];
    }
    avg_grade /= GRADES_COUNT;
    fprintf((FILE *)fout, "ID: %zu, Surname: ", s->id);
    string_fprint((FILE *)fout, s->last_name);
    fprintf((FILE *)fout, ", Name: ");
    string_fprint((FILE *)fout, s->name);
    fprintf((FILE *)fout, ", Group: ");
    string_fprint((FILE *)fout, s->group);
    fprintf((FILE *)fout, ", Grades: %u %u %u %u %u, Average grade: %lf\n",
            s->grades[0], s->grades[1], s->grades[2], s->grades[3],
            s->grades[4], avg_grade);
    return EXIT_SUCCESS;
}
err_t print_best_students(const FILE *fout, const Students_data *s) {
    size_t i, j;
    double avg = 0;

    if (fout == NULL || s == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    for (i = 0; i < s->size; ++i) {
        for (j = 0; j < GRADES_COUNT; ++j) {
            avg += s->students[i].grades[j];
        }
        avg /= GRADES_COUNT;
        if (avg > s->average_grade) {
            student_print(fout, s->students + i);
        }
    }
    return EXIT_SUCCESS;
}

void print_students_menu() {
    printf("Program Menu:\n");
    printf("1. Search student by ID\n");
    printf("2. Search student by last name\n");
    printf("3. Search student by first name\n");
    printf("4. Search student by group\n");
    printf("5. Sort students by ID\n");
    printf("6. Sort students by last name\n");
    printf("7. Sort students by first name\n");
    printf("8. Sort students by group\n");
    printf("9. Output student data to trace file by ID\n");
    printf("10. Output students with above-average grades to trace file\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}
