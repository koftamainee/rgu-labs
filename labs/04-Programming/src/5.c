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
    String birthday;
    size_t age;
    Gender gender;
    double salary;
} Citizen;

typedef enum undo_action { create, update, delete } undo_action;

typedef struct undo_entry {
    undo_action action;
    Citizen prev;
    Citizen current;
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
int citizen_equality_comparer(const void *a, const void *b);

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
    err = undostack_init(&us, sizeof(undo_entry), undo_entry_free);
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

        if (gender != 'M' && gender != 'W') {
            string_free(new->last_name);
            string_free(new->name);
            string_free(new->surname);
            free(new);
            return INVALID_INPUT_DATA;
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
        new->birthday = string_from(datetime);
        if (new->birthday == NULL) {
            string_free(new->last_name);
            string_free(new->name);
            string_free(new->surname);
            free(new);
            return MEMORY_ALLOCATION_ERROR;
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
    printf(" %c ", cp->gender == Male ? 'M' : 'W');
    string_print(cp->birthday);
    printf(", %zu y.o, Salary: %.2lf $\n", cp->age, cp->salary);
}

void citizen_free(void *c) {
    if (c == NULL) {
        return;
    }
    Citizen *cp = (Citizen *)c;
    string_free(cp->last_name);
    string_free(cp->name);
    string_free(cp->surname);
    string_free(cp->birthday);
    free(cp);
}

int citizen_age_comparer(const void *a, const void *b) {
    return ((const Citizen *)a)->age - ((const Citizen *)b)->age;
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
    int age;

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
    if (age < 0) {
        return INVALID_INPUT_DATA;
    }

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
        fflush(stdin);
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
                break;
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
    undo_entry *ue = NULL;
    char c;
    char c_ans[BUFSIZ];
    Citizen *new = NULL;
    String s_ans = NULL;
    err_t err;
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    new = (Citizen *)malloc(sizeof(Citizen));
    if (new == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    new->last_name = NULL;
    new->name = NULL;
    new->surname = NULL;

    fflush(stdin);

    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter last name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    new->last_name = s_ans;
    s_ans = NULL;

    printf("Enter name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    new->name = s_ans;
    s_ans = NULL;

    printf("Enter surname: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    new->surname = s_ans;
    s_ans = NULL;

    printf("Enter birth date: ");
    scanf("%s", c_ans);
    if (regex_date(c_ans) == 0) {
        free(new);
        return REGEX_FAILED;
    }
    err = calculate_age(c_ans, &new->age);
    if (err) {
        free(new);
        return err;
    }
    new->birthday = string_from(c_ans);
    if (new->birthday == NULL) {
        free(new);
        return MEMORY_ALLOCATION_ERROR;
    }

    fflush(stdin);
    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter gender: ");
    scanf("%c", &c);
    if (c != 'M' && c != 'W') {
        free(new);
        return INVALID_INPUT_DATA;
    }
    new->gender = c == 'M' ? Male : Female;

    printf("Enter salary: ");
    scanf("%lf", &new->salary);
    new->salary = new->salary > 0 ? new->salary : 0;

    err = u_list_insert_sorted(town, new, citizen_age_comparer);
    if (err) {
        free(new);
        return err;
    }
    if (err) {
        free(new);
        return err;
    }

    ue = (undo_entry *)malloc(sizeof(undo_entry));
    if (ue == NULL) {
        free(new);
        return MEMORY_ALLOCATION_ERROR;
    }
    ue->action = create;
    ue->current.age = new->age;
    ue->current.salary = new->salary;
    ue->current.gender = new->gender;
    ue->prev.last_name = NULL;
    ue->prev.name = NULL;
    ue->prev.surname = NULL;
    ue->prev.birthday = NULL;
    ue->current.last_name = string_init();
    ue->current.name = string_init();
    ue->current.surname = string_init();
    ue->current.birthday = string_init();
    if (ue->current.last_name == NULL || ue->current.name == NULL ||
        ue->current.surname == NULL || ue->current.birthday == NULL) {
        free(new);
        return MEMORY_ALLOCATION_ERROR;
    }
    string_cpy(&ue->current.last_name, &new->last_name);
    string_cpy(&ue->current.name, &new->name);
    string_cpy(&ue->current.surname, &new->surname);
    string_cpy(&ue->current.birthday, &new->birthday);

    free(new);

    err = u_list_insert(us->s, 0, ue);
    if (err) {
        undo_entry_free(ue);
        return err;
    }
    us->total_operation_count += 1;

    return EXIT_SUCCESS;
}

err_t read_citizen(u_list *town, undostack *us) {
    int search_flags[6] = {0, 0, 0, 0, 0, 0};
    Citizen *current_c;
    int gender_flag = 0;
    String s_ans = NULL;
    char c_ans[BUFSIZ];
    Citizen *new = NULL;
    err_t err;
    char c;
    u_list_node *current = NULL;
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    new = (Citizen *)malloc(sizeof(Citizen));
    if (new == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    new->last_name = NULL;
    new->name = NULL;
    new->surname = NULL;

    fflush(stdin);

    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter last name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    new->last_name = s_ans;
    if (s_ans[0] == '-') {
        search_flags[0] = 1;
    }
    s_ans = NULL;

    printf("Enter name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    if (s_ans[0] == '-') {
        search_flags[1] = 1;
    }
    new->name = s_ans;
    s_ans = NULL;

    printf("Enter surname: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    if (s_ans[0] == '-') {
        search_flags[2] = 1;
    }
    new->surname = s_ans;
    s_ans = NULL;

    printf("Enter birth date: ");
    scanf("%s", c_ans);
    if (c_ans[0] == '-') {
        search_flags[3] = 1;
        new->age = 0;
        new->birthday = NULL;
    } else {
        search_flags[3] = 0;
        if (regex_date(c_ans) == 0) {
            free(new);
            return REGEX_FAILED;
        }
        err = calculate_age(c_ans, &new->age);
        if (err) {
            free(new);
            return err;
        }
        new->birthday = string_from(c_ans);
        if (new->birthday == NULL) {
            citizen_free(new);
            return MEMORY_ALLOCATION_ERROR;
        }
    }
    fflush(stdin);
    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter gender: ");
    scanf("%c", &c);
    if (c == '-') {
        search_flags[4] = 1;
    } else {
        search_flags[4] = 0;
        if (c != 'M' && c != 'W') {
            free(new);
            return INVALID_INPUT_DATA;
        }
        new->gender = c == 'M' ? Male : Female;
    }

    printf("Enter salary: ");
    scanf("%s", c_ans);
    if (c_ans[0] == '-') {
        search_flags[5] = 1;
    }
    new->salary = atoi(c_ans);

    current = town->first;
    while (current != NULL) {
        current_c = (Citizen *)current->data;
        if ((search_flags[0] ||
             string_cmp(current_c->last_name, new->last_name) == 0) &&
            (search_flags[1] || string_cmp(current_c->name, new->name)) &&
            (search_flags[2] || string_cmp(current_c->surname, new->surname)) &&
            (search_flags[3] ||
             string_cmp(current_c->birthday, new->birthday)) &&
            (search_flags[4] || current_c->gender - new->gender == 0) &&
            (search_flags[5] || current_c->salary - new->salary == 0)) {
            print_citizen(current);
        }
        current = current->next;
    }
    free(new);

    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');
    getchar();

    return EXIT_SUCCESS;
}

err_t update_citizen(u_list *town, undostack *us) {
    double double_placeholder;
    size_t size_t_placeholder;
    undo_entry *ue = NULL;
    int changed = 0;
    int search_flags[6] = {0, 0, 0, 0, 0, 0};
    Citizen *current_c;
    int gender_flag = 0;
    String s_ans = NULL;
    char c_ans[BUFSIZ];
    Citizen *new = NULL;
    err_t err;
    char c;
    int user_input = -1;
    u_list_node *current = NULL;
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    new = (Citizen *)malloc(sizeof(Citizen));
    if (new == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    new->last_name = NULL;
    new->name = NULL;
    new->surname = NULL;

    fflush(stdin);

    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter last name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    new->last_name = s_ans;
    if (s_ans[0] == '-') {
        search_flags[0] = 1;
    }
    s_ans = NULL;

    printf("Enter name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    if (s_ans[0] == '-') {
        search_flags[1] = 1;
    }
    new->name = s_ans;
    s_ans = NULL;

    printf("Enter surname: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        free(new);
        return err;
    }
    if (s_ans[0] == '-') {
        search_flags[2] = 1;
    }
    new->surname = s_ans;
    s_ans = NULL;

    printf("Enter birth date: ");
    scanf("%s", c_ans);
    if (c_ans[0] == '-') {
        search_flags[3] = 1;
        new->age = 0;
        new->birthday = NULL;
    } else {
        search_flags[3] = 0;
        if (regex_date(c_ans) == 0) {
            free(new);
            return REGEX_FAILED;
        }
        err = calculate_age(c_ans, &new->age);
        if (err) {
            free(new);
            return err;
        }
        new->birthday = string_from(c_ans);
        if (new->birthday == NULL) {
            free(new);
            return MEMORY_ALLOCATION_ERROR;
        }
    }
    fflush(stdin);
    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter gender: ");
    scanf("%c", &c);
    if (c == '-') {
        search_flags[4] = 1;
    } else {
        search_flags[4] = 0;
        if (c != 'M' && c != 'W') {
            free(new);
            return INVALID_INPUT_DATA;
        }
        new->gender = c == 'M' ? Male : Female;
    }

    printf("Enter salary: ");
    scanf("%s", c_ans);
    if (c_ans[0] == '-') {
        search_flags[5] = 1;
    }
    new->salary = atoi(c_ans);

    current = town->first;
    while (current != NULL) {
        current_c = (Citizen *)current->data;
        if ((search_flags[0] ||
             string_cmp(current_c->last_name, new->last_name) == 0) &&
            (search_flags[1] || string_cmp(current_c->name, new->name)) &&
            (search_flags[2] || string_cmp(current_c->surname, new->surname)) &&
            (search_flags[3] ||
             string_cmp(current_c->birthday, new->birthday)) &&
            (search_flags[4] || current_c->gender - new->gender == 0) &&
            (search_flags[5] || current_c->salary - new->salary == 0)) {
            clear_screen();
            printf("Found: ");
            string_print(current_c->last_name);
            printf(" ");
            string_print(current_c->name);
            printf("\n");
            ue = (undo_entry *)malloc(sizeof(undo_entry));
            if (ue == NULL) {
                return MEMORY_ALLOCATION_ERROR;
            }
            ue->action = update;
            ue->prev.gender = current_c->gender;
            ue->prev.salary = current_c->salary;
            ue->prev.age = current_c->age;
            ue->prev.birthday = string_init();
            ue->prev.last_name = string_init();
            ue->prev.name = string_init();
            ue->prev.surname = string_init();

            ue->current.gender = current_c->gender;
            ue->current.salary = current_c->salary;
            ue->current.age = current_c->age;
            ue->current.birthday = string_init();
            ue->current.last_name = string_init();
            ue->current.name = string_init();
            ue->current.surname = string_init();
            if (ue->prev.birthday == NULL || ue->prev.last_name == NULL ||
                ue->prev.name == NULL || ue->prev.surname == NULL ||
                ue->current.birthday == NULL || ue->current.last_name == NULL ||
                ue->current.name == NULL || ue->current.surname == NULL) {
                undo_entry_free(ue);
                citizen_free(new);
                return MEMORY_ALLOCATION_ERROR;
            }
            err = string_cpy(&ue->prev.birthday, &current_c->birthday);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->prev.last_name, &current_c->last_name);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->prev.name, &current_c->name);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->prev.surname, &current_c->surname);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->current.birthday, &current_c->birthday);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->current.last_name, &current_c->last_name);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->current.name, &current_c->name);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->current.surname, &current_c->surname);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }

            while (user_input != 0) {
                printf(
                    "What do you want to change?\n"
                    "1. Last name.\n"
                    "2. Name.\n"
                    "3. Surname.\n"
                    "4. Birthday.\n"
                    "5. Gender.\n"
                    "6. Salary.\n"
                    "0. Next.\n"
                    "Choose: ");
                scanf("%d", &user_input);

                switch (user_input) {
                    case 0:
                        break;

                    case 1:
                        printf("Enter new last name: ");
                        scanf("%s", c_ans);
                        err = string_cpy_c(&current_c->last_name, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        err = string_cpy_c(&ue->current.last_name, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        changed = 1;
                        break;

                    case 2:
                        printf("Enter new name: ");
                        scanf("%s", c_ans);
                        err = string_cpy_c(&current_c->name, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        err = string_cpy_c(&ue->current.name, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        changed = 1;
                        break;

                    case 3:
                        printf("Enter new surname: ");
                        scanf("%s", c_ans);
                        err = string_cpy_c(&current_c->surname, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        err = string_cpy_c(&ue->current.surname, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        changed = 1;
                        break;

                    case 4:
                        printf("Enter new birthday: ");
                        scanf("%s", c_ans);
                        if (regex_date(c_ans) == 0) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return REGEX_FAILED;
                        }
                        err = calculate_age(c_ans, &size_t_placeholder);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        current_c->age = size_t_placeholder;
                        ue->current.age = size_t_placeholder;

                        err = string_cpy_c(&current_c->birthday, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }
                        err = string_cpy_c(&ue->current.birthday, c_ans);
                        if (err) {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return err;
                        }

                        changed = 1;
                        break;

                    case 5:
                        printf("Enter new gender: ");
                        scanf("%c", &c);
                        if (c != 'M' || c != 'W') {
                            undo_entry_free(ue);
                            citizen_free(new);
                            return INVALID_INPUT_DATA;
                        }
                        current_c->gender = c == 'M' ? Male : Female;
                        ue->current.gender = c == 'M' ? Male : Female;
                        changed = 1;
                        break;

                    case 6:
                        printf("Enter new salary: ");
                        scanf("%lf", &double_placeholder);
                        double_placeholder =
                            double_placeholder > 0 ? double_placeholder : 0;
                        current_c->salary = double_placeholder;
                        ue->current.salary = double_placeholder;
                        changed = 1;
                        break;
                }

                clear_screen();
            }
            user_input = -1;
            if (changed) {
                changed = 0;
                err = stack_push(us->s, ue);
                if (err) {
                    undo_entry_free(ue);
                    citizen_free(new);
                    return err;
                }
                us->total_operation_count++;
                free(ue);
            } else {
                undo_entry_free(ue);
            }
        }
        current = current->next;
    }
    citizen_free(new);

    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');
    getchar();

    return EXIT_SUCCESS;
}

err_t delete_citizen(u_list *town, undostack *us) {
    double double_placeholder;
    size_t size_t_placeholder;
    undo_entry *ue = NULL;
    int flag = 0;
    int search_flags[6] = {0, 0, 0, 0, 0, 0};
    Citizen *current_c;
    int gender_flag = 0;
    String s_ans = NULL;
    char c_ans[BUFSIZ];
    Citizen *new = NULL;
    err_t err;
    char c;
    int user_input = -1;
    u_list_node *current = NULL;
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    new = (Citizen *)malloc(sizeof(Citizen));
    if (new == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    new->last_name = NULL;
    new->name = NULL;
    new->surname = NULL;

    fflush(stdin);

    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter last name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        citizen_free(new);
        return err;
    }
    new->last_name = s_ans;
    if (s_ans[0] == '-') {
        search_flags[0] = 1;
    }
    s_ans = NULL;

    printf("Enter name: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        citizen_free(new);
        return err;
    }
    if (s_ans[0] == '-') {
        search_flags[1] = 1;
    }
    new->name = s_ans;
    s_ans = NULL;

    printf("Enter surname: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        citizen_free(new);
        return err;
    }
    if (s_ans[0] == '-') {
        search_flags[2] = 1;
    }
    new->surname = s_ans;
    s_ans = NULL;

    printf("Enter birth date: ");
    scanf("%s", c_ans);
    if (c_ans[0] == '-') {
        search_flags[3] = 1;
        new->age = 0;
        new->birthday = NULL;
    } else {
        search_flags[3] = 0;
        if (regex_date(c_ans) == 0) {
            citizen_free(new);
            return REGEX_FAILED;
        }
        err = calculate_age(c_ans, &new->age);
        if (err) {
            citizen_free(new);
            return err;
        }
        new->birthday = string_from(c_ans);
        if (new->birthday == NULL) {
            citizen_free(new);
            return MEMORY_ALLOCATION_ERROR;
        }
    }
    fflush(stdin);
    while (c != EOF && (c = getchar()) != '\n');
    printf("Enter gender: ");
    scanf("%c", &c);
    if (c == '-') {
        search_flags[4] = 1;
    } else {
        search_flags[4] = 0;
        if (c != 'M' && c != 'W') {
            citizen_free(new);
            return INVALID_INPUT_DATA;
        }
        new->gender = c == 'M' ? Male : Female;
    }

    printf("Enter salary: ");
    scanf("%s", c_ans);
    if (c_ans[0] == '-') {
        search_flags[5] = 1;
    }
    new->salary = atoi(c_ans);

    current = town->first;
    while (current != NULL) {
        current_c = (Citizen *)current->data;
        if ((search_flags[0] ||
             string_cmp(current_c->last_name, new->last_name) == 0) &&
            (search_flags[1] || string_cmp(current_c->name, new->name)) &&
            (search_flags[2] || string_cmp(current_c->surname, new->surname)) &&
            (search_flags[3] ||
             string_cmp(current_c->birthday, new->birthday)) &&
            (search_flags[4] || current_c->gender - new->gender == 0) &&
            (search_flags[5] || current_c->salary - new->salary == 0)) {
            printf("Deleting: ");
            string_print(current_c->last_name);
            printf(" ");
            string_print(current_c->name);
            printf("\n");
            ue = (undo_entry *)malloc(sizeof(undo_entry));
            if (ue == NULL) {
                return MEMORY_ALLOCATION_ERROR;
            }
            ue->action = delete;
            ue->prev.gender = current_c->gender;
            ue->prev.salary = current_c->salary;
            ue->prev.age = current_c->age;
            ue->prev.birthday = string_init();
            ue->prev.last_name = string_init();
            ue->prev.name = string_init();
            ue->prev.surname = string_init();

            ue->current.gender = 0;
            ue->current.salary = 0;
            ue->current.age = 0;
            ue->current.birthday = NULL;
            ue->current.last_name = NULL;
            ue->current.name = NULL;
            ue->current.surname = NULL;
            if (ue->prev.birthday == NULL || ue->prev.last_name == NULL ||
                ue->prev.name == NULL || ue->prev.surname == NULL) {
                undo_entry_free(ue);
                citizen_free(new);
                return MEMORY_ALLOCATION_ERROR;
            }
            err = string_cpy(&ue->prev.birthday, &current_c->birthday);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->prev.last_name, &current_c->last_name);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->prev.name, &current_c->name);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = string_cpy(&ue->prev.surname, &current_c->surname);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            current = current->next;
            flag = 1;
            err = u_list_delete_by_value(town, current_c,
                                         citizen_equality_comparer);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            err = stack_push(us->s, ue);
            if (err) {
                undo_entry_free(ue);
                citizen_free(new);
                return err;
            }
            us->total_operation_count++;
            free(ue);
        }
        if (flag == 0) {
            current = current->next;
        }
        flag = 0;
    }

    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');
    getchar();

    return EXIT_SUCCESS;
}

err_t output_to_file(u_list *town) {
    u_list_node *current = NULL;
    Citizen *cp;
    char c_ans[BUFSIZ];
    FILE *fout = NULL;
    if (town == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    printf("Enter file path: ");
    scanf("%s", c_ans);

    fout = fopen(c_ans, "w");
    if (fout == NULL) {
        return OPENING_THE_FILE_ERROR;
    }

    current = town->first;
    while (current != NULL) {
        cp = (Citizen *)current->data;
        string_fprint(fout, cp->last_name);
        fprintf(fout, " ");
        string_fprint(fout, cp->name);
        fprintf(fout, " ");
        string_fprint(fout, cp->surname);
        fprintf(fout, " %c ", cp->gender == Male ? 'M' : 'W');
        string_fprint(fout, cp->birthday);
        fprintf(fout, ", %zu y.o, Salary: %.2lf $\n", cp->age, cp->salary);
        current = current->next;
    }

    fclose(fout);
    return EXIT_SUCCESS;
}

err_t undo(u_list *town, undostack *us) {
    Citizen *to_update = NULL;
    u_list_node *ret = NULL;
    char c;
    err_t err;
    stack_item *popped;
    undo_entry *ue;
    if (town == NULL || us == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (us->total_operation_count / 2 < us->current_operation_count + 1) {
        printf("Can not undo more than N/2 operations\n");
        printf("\nPress enter to continue");
        while (c != EOF && (c = getchar()) != '\n');
        getchar();
        return EXIT_SUCCESS;
    }

    err = stack_top(us->s, &popped);
    if (err) {
        return err;
    }
    ue = (undo_entry *)popped->data;

    switch (ue->action) {
        case create:
            err = u_list_delete_by_value(town, &ue->current,
                                         citizen_equality_comparer);
            if (err) {
                return err;
            }
            break;

        case update:
            err = u_list_get_node_by_value(town, &ue->current,
                                           citizen_equality_comparer, &ret);
            if (err) {
                return err;
            }
            to_update = (Citizen *)ret->data;
            to_update->salary = ue->prev.salary;
            to_update->gender = ue->prev.gender;
            to_update->age = ue->prev.age;

            err = string_cpy(&to_update->last_name, &ue->prev.last_name);
            if (err) {
                return err;
            }
            err = string_cpy(&to_update->name, &ue->prev.name);
            if (err) {
                return err;
            }
            err = string_cpy(&to_update->surname, &ue->prev.surname);
            if (err) {
                return err;
            }
            err = string_cpy(&to_update->birthday, &ue->prev.birthday);
            if (err) {
                return err;
            }

            break;

        case delete:
            err = u_list_insert_sorted(town, &ue->prev,
                                       citizen_equality_comparer);
            if (err) {
                return err;
            }
            break;
    }

    err = stack_pop(us->s);
    if (err) {
        return err;
    }
    us->current_operation_count++;

    printf("Done.\n");
    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');
    getchar();

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
    return stack_init(&((*us)->s), element_size, elem_destructor);
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
    string_free(entry_p->current.last_name);
    string_free(entry_p->current.name);
    string_free(entry_p->current.surname);
    string_free(entry_p->current.birthday);

    string_free(entry_p->prev.last_name);
    string_free(entry_p->prev.name);
    string_free(entry_p->prev.surname);
    string_free(entry_p->prev.birthday);

    free(entry_p);
}

int citizen_equality_comparer(const void *a, const void *b) {
    Citizen *ap = (Citizen *)a;
    Citizen *bp = (Citizen *)b;
    int ret;
    ret = string_cmp(ap->last_name, bp->last_name);
    if (ret) {
        return ret;
    }
    ret = string_cmp(ap->name, bp->name);
    if (ret) {
        return ret;
    }
    ret = string_cmp(ap->surname, bp->surname);
    if (ret) {
        return ret;
    }
    ret = ap->gender - bp->gender;
    if (ret) {
        return ret;
    }
    ret = ap->age - bp->age;
    if (ret) {
        return ret;
    }
    ret = ap->salary - bp->salary;
    return ret;
}
