#include "../utils.h"

#include <stdlib.h>

int is_file_empty(FILE *fp) {
    long file_size, original_position;
    int current_position;

    original_position = ftell(fp);

    fseek(fp, 0, SEEK_END);

    file_size = ftell(fp);

    current_position = fseek(fp, original_position, SEEK_SET);
    if (file_size == 0) {
        return 1;
    }

    return 0;
}

err_t swap(void *first, void *second, size_t len) {
    if (first == NULL || second == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    int i;
    for (i = 0; i < len; ++i) {
        *((unsigned char *)first + i) ^= *((unsigned char *)second + i);
        *((unsigned char *)second + i) ^= *((unsigned char *)first + i);
        *((unsigned char *)first + i) ^= *((unsigned char *)second + i);
    }

    return EXIT_SUCCESS;
}
