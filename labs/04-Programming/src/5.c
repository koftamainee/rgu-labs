#include <stdlib.h>

#include "../../../libc/cstring.h"
#include "../../../libc/u_list.h"

typedef enum { Male, Female } Gender;

typedef struct Citizen {
    String last_name;
    String name;
    String lastname;
    Gender gender;
    double salary;
} Citizen;

int program_04_5(int argc, char *argv[]) {
    u_list_node *lp;
    err_t err;
    u_list l;
    err = u_list_init(&l, sizeof(int), free);
    if (err) {
        return err;
    }
    int a = 52;
    err = u_list_insert(&l, 0, &a);
    if (err) {
        return err;
    }
    err = u_list_insert(&l, 1, &a);
    if (err) {
        return err;
    }
    err = u_list_insert(&l, 52, &a);
    if (err) {
        return err;
    }

    err = u_list_get_node_by_index(&l, 1, &lp);
    if (err) {
        return err;
    }
    printf("%d\n%zu\n", *(int *)lp->data, l.size);

    u_list_free(&l);

    return EXIT_SUCCESS;
}
