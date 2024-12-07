#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libc/cstring.h"
#include "../../../libc/errors.h"

#define INITIAL_CAPACITY (16)

typedef struct Address {
    String city;
    String street;
    size_t house_number;
    String body;
    size_t flat_number;
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
err_t read_string_from_user(String *str);
err_t read_double_from_user(int *num);
err_t read_size_t_from_user(int *num);

// Creators
err_t post_init(Post *p);
err_t mail_init(Mail *m, String id);
err_t address_init(Address *a);

// Destructors
void post_destructor(Post *p);
void mail_destructor(Mail *m);
void address_destructor(Address *a);

// User Interfacue
err_t start_interactive_menu(Post *p);
err_t send_mail(Post *p);
err_t obtain_mail_by_id(Post *p);
err_t delete_mail(Post *p);
err_t get_delivered_mails(Post *p, Mail **m);
err_t get_not_delivered_mails(Post *p, Mail **m);

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

err_t read_string_from_user(String *str) {
    size_t len = 0;
    char buf[BUFSIZ];

    if (str == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (*str != NULL) {  // potentioal memory leak
        printf("WARNING: POTENCIAL MEMORY LEAK\n");
    }

    if (!fgets(buf, BUFSIZ, stdin)) {
        return ERROR_READING_FROM_STDIN;
    }
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len = 1] = '\0';  // delete '\n' symbol
    }

    *str = string_from(buf);  // allocating memory for string
    if (*str == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    return EXIT_SUCCESS;
}

err_t read_double_from_user(int *num) {
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

err_t read_size_t_from_user(int *num) {
    char buf[32];  // 32 bytes buf for int32_t
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
        printf("123");
        return MEMORY_ALLOCATION_ERROR;
    }
    p->mails = (Mail *)malloc(INITIAL_CAPACITY * sizeof(Mail));
    if (p->mails == NULL) {
        printf("12345");
        return MEMORY_ALLOCATION_ERROR;
    }

    p->mails_count = 0;
    p->__mails_capacity = INITIAL_CAPACITY;
    return EXIT_SUCCESS;
}

err_t mail_init(Mail *m, String id) {}

err_t address_init(Address *a) {}

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
}

err_t start_interactive_menu(Post *p) {}

err_t send_mail(Post *p) {}

err_t obtain_mail_by_id(Post *p) {}

err_t delete_mail(Post *p) {}

err_t get_delivered_mails(Post *p, Mail **m) {}

err_t get_not_delivered_mails(Post *p, Mail **m) {}
