#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"

void fatal_error(char *error_message) {
    fprintf(stderr, error_message);
    fprintf(stderr, "\n");
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

void print_args(Args args) {
    printf("arg flags:\n");
    printf("- dump header: %d\n", args.dump_header);

    printf("- file:\n");
    printf("\t- filename: %s\n", args.path.filepath);
    printf("\t- set: %d\n", args.path.set);
    printf("\t- len: %d\n", args.path.size);
}
