#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../headers/parse_args.h"


Args parse_headers(int argc, char *argv[]) {
    Args ret = {
        .dump_header = false,
    };

    for (int i = 1; i < argc; i++) {
        char *cur_arg = argv[i];

        if (strcmp(cur_arg, DUMP_HEADER_ARG) == 0) {
            ret.dump_header = true;
        } else {
            // assume is the filepath
            
        }
    }
}

bool file_exists(char *path) {
    if (access(path, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}
