#ifndef DUMP_SECTION_HEADER_H
#define DUMP_SECTION_HEADER_H

#include <stdint.h>

#include "../headers/misc.h"
#include "../headers/dump_header.h"


typedef struct section_header {
    char name[4];
    uint64_t type;
    uint64_t flags;
    uint64_t addr;
    uint64_t offset;
    uint64_t size;
    uint64_t link;
    uint64_t info;
    uint64_t align;
    uint64_t entsize;
} Section_header;

void navigate_fd_to_section_header(Elf_header header, FILE *fd);
Section_header grab_sect_header(Elf_header header, Args args);

#endif
