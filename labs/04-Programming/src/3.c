#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../libc/cstring.h"
#include "../../../libc/errors.h"
#include "../../../libc/input.h"
#include "../../../libc/memory.h"

#define INITIAL_CAPACITY (16)
#define GROWTH_FACTOR (2)

typedef struct Address {
    String city;
    String street;
    size_t house_number;
    String body;
    size_t flat_number;
    String index;
} Address;

typedef struct Mail {
    String id;
    Address recipient_adress;
    double weight;
    String creation_time;
    String delivery_time;
} Mail;

typedef struct Post {
    Address *post_office_address;
    Mail *mails;
    size_t mails_count;
    size_t __mails_capacity;
} Post;

// CLI utils
err_t read_double_from_user(double *num);
err_t read_size_t_from_user(size_t *num);
err_t print_mail(Mail *m);

// Creators
err_t post_init(Post *p);

// Utils
int post_comparer(const void *a, const void *b);
int by_time_comparer(const void *a, const void *b);

// Destructors
void post_destructor(Post *p);
void mail_destructor(Mail *m);
void address_destructor(Address *a);

// User Interfacue
err_t start_interactive_menu(Post *p);
err_t send_mail(Post *p);
err_t obtain_mail_by_id(Post *p);
err_t delete_mail(Post *p);
err_t get_delivered_mails(Post *p);
err_t get_not_delivered_mails(Post *p);

int program_04_3(int argc, char *argv[]) {
    err_t err = 0;
    Post p;

    err = post_init(&p);
    if (err) {
        post_destructor(&p);
        return err;
    }

    err = start_interactive_menu(&p);
    if (err) {
        post_destructor(&p);
        return err;
    }

    post_destructor(&p);
    return EXIT_SUCCESS;
}

err_t read_double_from_user(double *num) {
    char buf[64];  // 32 bytes buf for int32_t
    char *endptr = NULL;

    if (num == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (!fgets(buf, sizeof(buf), stdin)) {
        return ERROR_READING_FROM_STDIN;
    }

    *num = strtol(buf, &endptr, 10);

    return EXIT_SUCCESS;
}

err_t read_size_t_from_user(size_t *num) {
    char buf[64];  // 32 bytes buf for int32_t
    char *endptr = NULL;

    if (num == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (!fgets(buf, sizeof(buf), stdin)) {
        return ERROR_READING_FROM_STDIN;
    }

    *num = atoi(buf);

    return EXIT_SUCCESS;
}

err_t post_init(Post *p) {
    if (p == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    p->post_office_address = (Address *)malloc(sizeof(Address));
    if (p->post_office_address == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    p->mails = (Mail *)malloc(INITIAL_CAPACITY * sizeof(Mail));
    if (p->mails == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    p->mails_count = 0;
    p->__mails_capacity = INITIAL_CAPACITY;
    return EXIT_SUCCESS;
}

void post_destructor(Post *p) {
    size_t i = 0;
    address_destructor(p->post_office_address);
    for (i = 0; i < p->mails_count; ++i) {
        mail_destructor(p->mails + i);
    }
    free(p->mails);
}

void mail_destructor(Mail *m) {
    string_free(m->id);
    address_destructor(&(m->recipient_adress));
    string_free(m->creation_time);
    string_free(m->delivery_time);
}

void address_destructor(Address *a) {
    string_free(a->city);
    string_free(a->street);
    string_free(a->body);
    string_free(a->index);
}

err_t start_interactive_menu(Post *p) {
    char c;
    err_t err;
    size_t user_ans;
    if (p == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    while (1) {
        clear_screen();
        printf(
            "Choose option: \n"
            "1. Send mail.\n"
            "2. Obtain mail by id.\n"
            "3. Delete mail by id.\n"
            "4. Get delivered mail.\n"
            "5. Get not delivered mail.\n"
            "0. Exit\n\nInput number: ");
        while (c != EOF && (c = getchar()) != '\n');
        err = read_size_t_from_user(&user_ans);
        if (err) {
            return err;
        }
        switch (user_ans) {
            case 1:
                err = send_mail(p);
                if (err) {
                    return err;
                }
                break;
            case 2:
                err = obtain_mail_by_id(p);
                if (err) {
                    return err;
                }
                break;
            case 3:
                err = delete_mail(p);
                if (err) {
                    return err;
                }
                break;
            case 4:
                err = get_delivered_mails(p);
                if (err) {
                    return err;
                }
                break;
            case 5:
                err = get_not_delivered_mails(p);
                if (err) {
                    return err;
                }
                break;
            case 0:
                return EXIT_SUCCESS;
            default:
                return INVALID_FLAG;
        }
        fflush(stdin);
    }
    return EXIT_SUCCESS;
}

err_t send_mail(Post *p) {
    String s_ans = NULL;
    size_t st_ans = 0;
    double d_ans;

    err_t err;

    if (p->mails_count + 1 > p->__mails_capacity) {
        err = rerealloc((void **)&(p->mails),
                        p->__mails_capacity * GROWTH_FACTOR * sizeof(Mail));
        if (err) {
            return err;
        }
        p->__mails_capacity *= GROWTH_FACTOR;
    }

    // p->mails[p->mails_count].id;
    printf("Enter id: ");
    fflush(stdin);
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    p->mails[p->mails_count].id = s_ans;
    s_ans = NULL;

    printf("RECIPIENT INFO\n");
    printf("Enter city: ");
    fflush(stdin);
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    p->mails[p->mails_count].recipient_adress.city = s_ans;
    s_ans = NULL;
    printf("Enter street: ");
    fflush(stdin);
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    p->mails[p->mails_count].recipient_adress.street = s_ans;
    s_ans = NULL;

    printf("Enter house number: ");
    fflush(stdin);
    err = read_size_t_from_user(&st_ans);
    if (err) {
        return err;
    }
    p->mails[p->mails_count].recipient_adress.house_number = st_ans;
    st_ans = 0;

    printf("Enter body: ");
    fflush(stdin);
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    p->mails[p->mails_count].recipient_adress.body = s_ans;
    s_ans = NULL;

    printf("Enter flat number: ");
    fflush(stdin);
    err = read_size_t_from_user(&st_ans);
    if (err) {
        return err;
    }
    p->mails[p->mails_count].recipient_adress.flat_number = st_ans;
    st_ans = 0;

    printf("Enter index: ");
    fflush(stdin);
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    p->mails[p->mails_count].recipient_adress.index = s_ans;
    s_ans = NULL;

    printf("Enter package weight: ");
    fflush(stdin);
    err = read_double_from_user(&d_ans);
    if (err) {
        return err;
    }
    p->mails[p->mails_count].weight = d_ans;
    d_ans = 0;

    printf("Enter creation time: ");
    fflush(stdin);
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    p->mails[p->mails_count].creation_time = s_ans;
    s_ans = NULL;

    printf("Enter delivery time: ");
    fflush(stdin);
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    p->mails[p->mails_count].delivery_time = s_ans;
    s_ans = NULL;

    p->mails_count++;

    qsort((void *)p->mails, p->mails_count, sizeof(Mail), post_comparer);

    return EXIT_SUCCESS;
}

err_t obtain_mail_by_id(Post *p) {
    char c;
    err_t err;
    String s_ans = NULL;
    size_t i;

    printf("Enter id: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    for (i = 0; i < p->mails_count; ++i) {
        if (!string_cmp(s_ans, p->mails[i].id)) {
            err = print_mail(p->mails + i);
            if (err) {
                return err;
            }
            printf("\nPress enter to continue");
            while (c != EOF && (c = getchar()) != '\n');
            string_free(s_ans);
            return EXIT_SUCCESS;
        }
    }
    printf("Mail with ");
    string_print(s_ans);
    printf(" id not exits\n");
    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');
    string_free(s_ans);
    return EXIT_SUCCESS;
}

err_t delete_mail(Post *p) {
    char c;
    err_t err;
    String s_ans = NULL;
    size_t i;

    printf("Enter id: ");
    err = read_string_from_user(&s_ans);
    if (err) {
        string_free(s_ans);
        return err;
    }
    for (i = 0; i < p->mails_count; ++i) {
        if (!string_cmp(s_ans, p->mails[i].id)) {
            memmove(p->mails + i, p->mails + i + 1,
                    sizeof(Mail) * (p->mails_count - i - 1));
            p->mails_count--;
            printf("Mail deleted\n");
            printf("\nPress enter to continue");
            while (c != EOF && (c = getchar()) != '\n');
            string_free(s_ans);
            return EXIT_SUCCESS;
        }
    }

    printf("Mail with ");
    string_print(s_ans);
    printf(" id not exits\n");
    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');
    string_free(s_ans);
    return EXIT_SUCCESS;
}

err_t get_delivered_mails(Post *p) {
    err_t err;
    size_t i;
    char time_buf[64], c;
    String local_time = NULL;
    Mail *sorted;
    time_t t = time(NULL);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&t));

    sorted = (Mail *)malloc(sizeof(Mail) * p->mails_count);
    if (sorted == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    local_time = string_from(time_buf);
    if (local_time == NULL) {
        free(sorted);
        return MEMORY_ALLOCATION_ERROR;
    }

    memcpy(sorted, p->mails, sizeof(Mail) * p->mails_count);
    qsort((void *)p->mails, p->mails_count, sizeof(Mail), by_time_comparer);
    for (i = 0; i < p->mails_count; ++i) {
        if (string_cmp(local_time, p->mails[i].delivery_time) <= 0) {
            err = print_mail(p->mails + i);
            if (err) {
                return err;
            }
        }
    }

    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');

    free(sorted);
    string_free(local_time);
    return EXIT_SUCCESS;
}

err_t get_not_delivered_mails(Post *p) {
    err_t err;
    size_t i;
    char time_buf[64], c;
    String local_time = NULL;
    Mail *sorted;
    time_t t = time(NULL);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&t));

    sorted = (Mail *)malloc(sizeof(Mail) * p->mails_count);
    if (sorted == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    local_time = string_from(time_buf);
    if (local_time == NULL) {
        free(sorted);
        return MEMORY_ALLOCATION_ERROR;
    }

    memcpy(sorted, p->mails, sizeof(Mail) * p->mails_count);
    qsort((void *)p->mails, p->mails_count, sizeof(Mail), by_time_comparer);
    for (i = 0; i < p->mails_count; ++i) {
        if (string_cmp(local_time, p->mails[i].delivery_time) > 0) {
            err = print_mail(p->mails + i);
            if (err) {
                return err;
            }
        }
    }

    printf("\nPress enter to continue");
    while (c != EOF && (c = getchar()) != '\n');

    free(sorted);
    string_free(local_time);
    return EXIT_SUCCESS;
}

int post_comparer(const void *a, const void *b) {
    int ret;
    Mail *ap = (Mail *)a;
    Mail *bp = (Mail *)b;
    ret = string_cmp(ap->recipient_adress.index, bp->recipient_adress.index);
    if (ret) {  // strings not equal
        return ret;
    }
    ret = string_cmp(ap->id, bp->id);
    return ret;
}

int by_time_comparer(const void *a, const void *b) {
    Mail *ap = (Mail *)a;
    Mail *bp = (Mail *)b;
    return string_cmp(ap->creation_time, bp->creation_time);
}

err_t print_mail(Mail *m) {
    if (m == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    printf("Mail with id ");
    string_print(m->id);
    printf("\nWeigth: %lf\nCreation time: ", m->weight);
    string_print(m->creation_time);
    printf("\nDelivery time: ");
    string_print(m->delivery_time);
    printf("\nRecepient: \nCity: ");
    string_print(m->recipient_adress.city);
    printf("\nStreet: ");
    string_print(m->recipient_adress.street);
    printf("\nHouse number: %zu\nBody: ", m->recipient_adress.house_number);
    string_print(m->recipient_adress.body);
    printf("\nFlat number: %zu\nIndex: ", m->recipient_adress.flat_number);
    string_print(m->recipient_adress.index);
    return EXIT_SUCCESS;
}
