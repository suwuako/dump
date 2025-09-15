#ifndef MISC_H
#define MISC_H

#include <stdbool.h>


void fatal_error(char *error_message);
bool file_exists(char *path);
bool file_readable(char *path);

#endif
