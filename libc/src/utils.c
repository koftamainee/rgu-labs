#include "../utils.h"

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
