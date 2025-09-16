#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <stdbool.h>


#define DUMP_HEADER_ARG "-h"

typedef struct filepath {
    char *filepath;
    int size;
    bool set;             // is set when valid filepath is set
} Filepath;


typedef struct args {
    Filepath path;
    bool dump_header;
} Args;


Args parse_args(int argc, char *argv[]);

#endif
