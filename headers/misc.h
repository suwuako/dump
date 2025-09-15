#ifndef MISC_H
#define MISC_H

#include <stdbool.h>

#include "parse_args.h"


void fatal_error(char *error_message);
bool file_exists(char *path);
bool file_readable(char *path);
void print_args(Args args);

#endif
