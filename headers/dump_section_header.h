#ifndef DUMP_SECTION_HEADER_H
#define DUMP_SECTION_HEADER_H

#include <stdint.h>

#include "../headers/misc.h"
#include "../headers/dump_elf_header.h"


#define VARIABLE_32BIT_SIZE 4
#define VARIABLE_64BIT_SIZE 8

typedef struct section_header {
    uint64_t sh_name;
    uint64_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint64_t sh_link;
    uint64_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} Section_header;

void navigate_fd_to_section_header(Elf_header header, FILE *fd);
void navigate_fd_to_section_index(Elf_header header, FILE *fd, int index);
Section_header grab_sect_header(Elf_header header, Args args, int index);
uint64_t read_nbytes_better(Elf_header header, FILE *fd, int bytes, bool variable);
void DEBUG_DUMP_NBYTES(int offset, int n, Args args);
Section_header *grab_all_section_headers(Elf_header header, Args args);
void dump_section_headers(Section_header *headers, Elf_header elf_header);
void print_and_format_section_header(Section_header shname, Section_header h, Elf_header elf_header, int i);

#endif
