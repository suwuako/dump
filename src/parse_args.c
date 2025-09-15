#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"


Args parse_args(int argc, char *argv[]) {
    Args ret = {
        .dump_header = false,
        .path = {
            .set = false,
        },
    };

    for (int i = 1; i < argc; i++) {
        char *cur_arg = argv[i];

        if (strcmp(cur_arg, DUMP_HEADER_ARG) == 0) {
            ret.dump_header = true;
        } else {
            // assume is the filepath if not a flag
            if (!file_exists(cur_arg)) { fatal_error("ERROR: filepath doesn't exist!"); }
            if (!file_readable(cur_arg)) { fatal_error("ERROR: unable to read file!"); }

            int len = strlen(cur_arg);
            char *filepath = malloc(sizeof(char) * len);
            strcpy(filepath, cur_arg);

            ret.path.size = len;
            ret.path.filepath = filepath;
            ret.path.set = true;
        }
    }

    if (!ret.path.set) { fatal_error("ERROR: user did not supply object filepath"); }

    return ret;
}
