#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../libc/cstring.h"
#include "../../../libc/input.h"
#include "../../../libc/stack.h"
#include "../../../libc/u_list.h"
#include "../../../libc/utils.h"

typedef enum { Male, Female } Gender;

typedef struct Citizen {
    String last_name;
    String name;
    String surname;
    size_t age;
    Gender gender;
    double salary;
} Citizen;

typedef enum undo_action { create, update, delete } undo_action;

typedef struct undo_entry {
    undo_action action;
    Citizen *c;
} undo_entry;

typedef struct undostack {
    stack *s;
    size_t current_operation_count;
    size_t total_operation_count;
} undostack;

// serialization
err_t citizens_deserialize(u_list *town, const char *filename);
err_t citizens_serialize(u_list *town, const char *filename);

// UI stuff
err_t start_citizens_menu(u_list *town, undostack *us);
/* ---------- CRUD moment..? ---------- */
err_t create_citizen(u_list *town, undostack *us);
err_t read_citizen(u_list *town, undostack *us);
err_t update_citizen(u_list *town, undostack *us);
err_t delete_citizen(u_list *town, undostack *us);
/* ---------- CRUD moment..? ---------- */
err_t output_to_file(u_list *town);
err_t undo(u_list *town, undostack *us);

// comparer
int citizen_age_comparer(const void *a, const void *b);

// memory stuff
// err_t citizen_init(Citizen **c, char *last_name, char *name, char *surname,
//                    Gender gender, double salary);
void citizen_free(void *c);

err_t undostack_init(undostack **us, size_t element_size,
                     void (*elem_destructor)(void *));
void undostack_free(undostack *us);
void undo_entry_free(void *entry);

// utils
void print_citizen(const u_list_node *n);
int regex_date(const char *date);
err_t calculate_age(const char *birthday, size_t *age_holder);

int program_04_5(int argc, char *argv[]) {
    err_t err;
    u_list *town = NULL;
    undostack *us = NULL;
    err = u_list_init(&town, sizeof(Citizen), citizen_free);
    if (err) {
        u_list_free(town);
        return err;
    }
    err = undostack_init(&us, sizeof(undo_action), undo_entry_free);
    if (err) {
        u_list_free(town);
        return err;
    }

    err = citizens_deserialize(town, "files/city.txt");
    if (err) {
        undostack_free(us);
        u_list_free(town);
        return err;
    }

    err = start_citizens_menu(town, us);
    if (err) {
        undostack_free(us);
        u_list_free(town);
        return err;
    }
    undostack_free(us);
    u_list_free(town);
    return EXIT_SUCCESS;
}

err_t citizens_deserialize(u_list *town, const char *filename) {
    err_t err;
    size_t cnt = 0;
    Citizen *new;
    FILE *fin = NULL;
    char gender;
    double salary = -52;
    char last_name[BUFSIZ], name[BUFSIZ], surname[BUFSIZ], datetime[BUFSIZ];
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return OPENING_THE_FILE_ERROR;
    }
    if (is_file_empty(fin)) {
        fclose(fin);
        return EXIT_SUCCESS;
    }

    while (fscanf(fin, "%s %s %s %s %c %lf", last_name, name, surname, datetime,
                  &gender, &salary) == 6) {
        new = (Citizen *)malloc(sizeof(Citizen));
        if (new == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        new->last_name = string_from(last_name);
        if (new->last_name == NULL) {
            free(new);
            return MEMORY_ALLOCATION_ERROR;
        }
        new->name = string_from(name);
        if (new->name == NULL) {
            string_free(new->last_name);
            free(new);
            return MEMORY_ALLOCATION_ERROR;
        }
        new->surname = string_from(surname);
        if (new->surname == NULL) {
            string_free(new->last_name);
            string_free(new->name);
            string_free(new->surname);
            free(new);
            return MEMORY_ALLOCATION_ERROR;
        }
        new->gender = gender == 'M' ? Male : Female;
        if (regex_date(datetime) == 0) {
            string_free(new->last_name);
            string_free(new->name);
            string_free(new->surname);
            free(new);
            return REGEX_FAILED;
        }
        err = calculate_age(datetime, &new->age);
        if (err) {
            string_free(new->last_name);
            string_free(new->name);
            string_free(new->surname);
            free(new);
            return err;
        }
        new->salary = salary > 0 ? salary : 0;
        err = u_list_insert_sorted(town, new, citizen_age_comparer);
        if (err) {
            string_free(new->last_name);
            string_free(new->name);
            string_free(new->surname);
            free(new);
            return err;
        }
        free(new);  // just free, we copyied pointers to strings
        new = NULL;
        cnt++;
    }
    fclose(fin);
    return EXIT_SUCCESS;
}

void print_citizen(const u_list_node *n) {
    if (n == NULL) {
        return;
    }
    Citizen *cp = (Citizen *)n->data;
    string_print(cp->last_name);
    printf(" ");
    string_print(cp->name);
    printf(" ");
    string_print(cp->surname);
    printf(" %c, %zu y.o, Salary: %.2lf $\n", cp->gender == Male ? 'M' : 'W',
           cp->age, cp->salary);
}

void citizen_free(void *c) {
    Citizen *cp = (Citizen *)c;
    string_free(cp->last_name);
    string_free(cp->name);
    string_free(cp->surname);
    free(cp);
}

int citizen_age_comparer(const void *a, const void *b) {
    return ((Citizen *)a)->age - ((Citizen *)b)->age;
}

int regex_date(const char *date) {
    // date = "DD.MM.YYYY"
    int day = -1, month = -1, year = -1;
    char day_s[3], month_s[3], year_s[5];  // +1 symbol for '\0'
    if (date[10] != '\0') {                // strlen() call may be undefined
        return 0;
    }
    if (date[2] != '.' || date[5] != '.') {
        return 0;
    }

    day_s[0] = date[0];
    day_s[1] = date[1];
    day_s[2] = '\0';

    month_s[0] = date[3];
    month_s[1] = date[4];
    month_s[2] = '\0';

    year_s[0] = date[6];
    year_s[1] = date[7];
    year_s[2] = date[8];
    year_s[3] = date[9];
    year_s[4] = '\0';

    day = atoi(day_s);
    month = atoi(month_s);
    year = atoi(year_s);
    if (day < 0 || day > 31) {
        return 0;
    }
    if (month < 0 || month > 12) {
        return 0;
    }
    if (year < 0 || year > 9999) {
        return 0;
    }

    return 1;
}

err_t calculate_age(const char *birthday, size_t *age_holder) {
    char day_s[3], month_s[3], year_s[5];  // +1 symbol for '\0'
    time_t now;
    struct tm *current_date;
    int current_day, current_month, current_year, birthday_day, birthday_month,
        birthday_year;
    size_t age;

    if (birthday == NULL || age_holder == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    now = time(NULL);
    current_date = localtime(&now);
    current_year = current_date->tm_year + 1900;
    current_month = current_date->tm_mon + 1;
    current_day = current_date->tm_mday;

    day_s[0] = birthday[0];
    day_s[1] = birthday[1];
    day_s[2] = '\0';

    month_s[0] = birthday[3];
    month_s[1] = birthday[4];
    month_s[2] = '\0';

    year_s[0] = birthday[6];
    year_s[1] = birthday[7];
    year_s[2] = birthday[8];
    year_s[3] = birthday[9];
    year_s[4] = '\0';

    birthday_day = atoi(day_s);
    birthday_month = atoi(month_s);
    birthday_year = atoi(year_s);

    age = current_year - birthday_year;

    if (current_month < birthday_month ||
        (current_month == birthday_month && current_day < birthday_day)) {
        age--;
    }

    *age_holder = age;

    return EXIT_SUCCESS;
}

err_t start_citizens_menu(u_list *town, undostack *us) {
    char c;
    err_t err;
    unsigned int user_input = -1;
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    while (user_input != 0) {
        clear_screen();
        printf(
            "1. Create citizen.\n"
            "2. Read Citizen.\n"
            "3. Update Citizen.\n"
            "4. Delete Citizen. (cruel)\n"
            "5. Serialize citizens to file\n"
            "6. Undo (f*ck)\n"
            "0. Exit.\n"
            "Choose: ");
        scanf("%u", &user_input);

        switch (user_input) {
            case 0:
                break;
            case 1:
                err = create_citizen(town, us);
                if (err) {
                    return err;
                }
            case 2:
                err = read_citizen(town, us);
                if (err) {
                    return err;
                }
                break;
            case 3:
                err = update_citizen(town, us);
                if (err) {
                    return err;
                }
                break;
            case 4:
                err = delete_citizen(town, us);
                if (err) {
                    return err;
                }
                break;
            case 5:
                err = output_to_file(town);
                if (err) {
                    return err;
                }
                break;
            case 6:
                err = undo(town, us);
                if (err) {
                    return err;
                }
                break;
            default:
                printf("Invalid u ii a i a ui i i a i\n");
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
        }
    }

    return EXIT_SUCCESS;
}

err_t create_citizen(u_list *town, undostack *us) {
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    return EXIT_SUCCESS;
}

err_t read_citizen(u_list *town, undostack *us) {
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    return EXIT_SUCCESS;
}

err_t update_citizen(u_list *town, undostack *us) {
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    return EXIT_SUCCESS;
}

err_t delete_citizen(u_list *town, undostack *us) {
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    return EXIT_SUCCESS;
}

err_t output_to_file(u_list *town) {
    if (town == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    return EXIT_SUCCESS;
}

err_t undo(u_list *town, undostack *us) {
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    return EXIT_SUCCESS;
}

err_t undostack_init(undostack **us, size_t element_size,
                     void (*elem_destructor)(void *)) {
    if (us == NULL || elem_destructor == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    *us = (undostack *)malloc(sizeof(undostack));
    if (*us == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    (*us)->total_operation_count = 0;
    (*us)->current_operation_count = 0;
    return stack_init(&((*us)->s), sizeof(undo_action), elem_destructor);
}
void undostack_free(undostack *us) {
    if (us == NULL) {
        return;
    }
    stack_free(us->s);
    free(us);
}
void undo_entry_free(void *entry) {
    if (entry == NULL) {
        return;
    }
    undo_entry *entry_p = (undo_entry *)entry;
    citizen_free(entry_p->c);
    free(entry_p);
}
