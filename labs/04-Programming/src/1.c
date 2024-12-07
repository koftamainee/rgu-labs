#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/cstring.h"
#include "../../../libs/errors.h"
#include "../../../libs/memory.h"

#define INITIAL_CAPACITY (16)
#define GROWTH_FACTOR (2)

typedef struct {
    size_t id;
    String name;
    String surname;
    double salary;
} Employee;

int read_employees_from_file(char *filename, Employee **destination,
                             size_t *employes_counter);
int ascending_comparer(const void *a, const void *b);
int descending_comparer(const void *a, const void *b);

int program_04_1(int argc, char *argv[]) {
    int err;
    size_t len, i;
    Employee *arr = NULL;

    if (argc != 2) {
        return INVALID_FLAG;
    }
    if (argv[1][0] == '-' || argv[1][0] == '/') {
        if (argv[1][1] == 'a') {
            err = read_employees_from_file("files/employees.txt", &arr, &len);
            if (err) {
                free(arr);
                return err;
            }
            qsort((void *)arr, len, sizeof(Employee), ascending_comparer);
        } else if (argv[1][1] == 'd') {
            err = read_employees_from_file("files/employees.txt", &arr, &len);
            if (err) {
                free(arr);
                return err;
            }
            qsort((void *)arr, len, sizeof(Employee), descending_comparer);
        }
    } else {
        return INVALID_FLAG;
    }
    printf("Sorted:\n");
    for (i = 0; i < len; ++i) {
        printf("Id: %zu, name: ", arr[i].id);
        string_print(arr[i].name);
        printf(", surname: ");
        string_print(arr[i].surname);
        printf(", salary: %lf$\n", arr[i].salary);
    }

    free(arr);
    return EXIT_SUCCESS;
}

int read_employees_from_file(char *filename, Employee **destination,
                             size_t *employes_counter) {
    FILE *fin = NULL;
    size_t id_buf, size = 0, capacity = INITIAL_CAPACITY;
    char name_buf[BUFSIZ];
    char surname_buf[BUFSIZ];
    double salary_buf;
    int err;

    if (filename == NULL || destination == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return OPENING_THE_FILE_ERROR;
    }

    *(destination) = (Employee *)malloc(sizeof(Employee) * capacity);
    if (*destination == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    while (fscanf(fin, "%zu %s %s %lf", &id_buf, name_buf, surname_buf,
                  &salary_buf) == 4) {
        if (size + 1 > capacity) {
            err = rerealloc((void **)destination,
                            capacity * GROWTH_FACTOR * sizeof(Employee));
            if (err) {
                return err;
            }
            capacity *= GROWTH_FACTOR;
        }

        (*destination)[size].id = id_buf;
        (*destination)[size].name = string_from(name_buf);
        (*destination)[size].surname = string_from(surname_buf);
        (*destination)[size].salary = salary_buf;
        if ((*destination)[size].name == NULL ||
            (*destination)[size].surname == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        size++;
    }
    fclose(fin);

    if (size < capacity) {
        err = rerealloc((void **)destination, size * sizeof(Employee));
        if (err) {
            return err;
        }
    }

    printf("%zu\n", size);
    *employes_counter = size;

    return EXIT_SUCCESS;
}

int ascending_comparer(const void *a, const void *b) {
    Employee *a_ptr = (Employee *)a;
    Employee *b_ptr = (Employee *)b;
    int res = 0;

    if (a_ptr->salary != b_ptr->salary) {
        return a_ptr->salary - b_ptr->salary;
    } else {
        res = string_cmp(a_ptr->surname, b_ptr->surname);
        if (res) {
            return res;
        } else {
            res = string_cmp(a_ptr->name, b_ptr->name);
            if (res) {
                return res;
            } else {
                return a_ptr->id - b_ptr->id;
            }
        }
    }
}
int descending_comparer(const void *a, const void *b) {
    Employee *a_ptr = (Employee *)a;
    Employee *b_ptr = (Employee *)b;
    int res = 0;

    if (a_ptr->salary != b_ptr->salary) {
        return b_ptr->salary - a_ptr->salary;
    } else {
        res = string_cmp(b_ptr->surname, a_ptr->surname);
        if (res) {
            return res;
        } else {
            res = string_cmp(b_ptr->name, a_ptr->name);
            if (res) {
                return res;
            } else {
                return b_ptr->id - a_ptr->id;
            }
        }
    }
}
