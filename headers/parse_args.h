#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <stdbool.h>


#define DUMP_HEADER_ARG "-dh"

typedef struct filepath {
    char *filepath;
    int size;
} Filepath;


typedef struct args {
    Filepath path;
    bool dump_header;
} Args;


Args parse_headers(int argc, char *argv[]);

#endif
