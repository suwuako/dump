#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../headers/misc.h"

void fatal_error(char *error_message) {
    fprintf(stderr, error_message);
    exit(1);
}


bool file_readable(char *path) {
    if (access(path, R_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

bool file_exists(char *path) {
    if (access(path, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}
