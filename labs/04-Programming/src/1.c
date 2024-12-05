#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/cstring.h"
#include "../../../libs/errors.h"
#include "../../../libs/memory.h"

typedef struct {
    size_t id;
    String name;
    String surname;
    size_t salary;
} Employee;

int read_employes_from_file(char *filename, Employee **destination);
int sort_employes(Employee **array);

int program_04_1(int argc, char *argv[]) { return EXIT_SUCCESS; }

int read_employes_from_file(char *filename, Employee **destination) {
    FILE *fin = NULL;
    size_t id_buf;
    char name_buf[BUFSIZ];
    char surname_buf[BUFSIZ];
    size_t salary_buf;

    if (filename == NULL || destination == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return OPENING_THE_FILE_ERROR;
        int read_employes_from_file(char *filename, Employee **destination);
        int sort_employes(Employee * *array);
    }

    while (!feof(fin)) {
        fscanf(fin, "%zu %s %s %zu", &id_buf, name_buf, surname_buf,
               &salary_buf);
    }

    return EXIT_SUCCESS;
}
int sort_employes(Employee **array) { return EXIT_SUCCESS; }
