#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "../headers/dump_section_header.h"
#include "../headers/dump_program_header.h"
#include "../headers/dump_elf_header.h"

// e_phnum Contains the number of entries in the program header table. 
// if e_phnum is 0, program header doesn't exist
bool program_header_exists(Elf_header header) {
    if (header.e_phnum == 0) return false;
    return true;
}

void navigate_fd_to_program_header(Elf_header header, FILE *fd) {
    uint64_t entry_point = header.e_phoff;

    lseek(fileno(fd), entry_point, SEEK_SET);
}

void navigate_fd_to_program_index(Elf_header header, FILE *fd, int i) {
    lseek(fileno(fd), i * header.e_phentsize, SEEK_CUR);
}

Program_header *grab_program_headers(Elf_header header, Args args) {
    Program_header *ret = calloc(sizeof(Program_header), header.e_phnum);

    for (int i = 0; i < header.e_phnum; i++) {
        Program_header prog = grab_program_header(header, args, i);
        ret[i] = prog;
    }

    return ret;
}

Program_header grab_program_header(Elf_header header, Args args, int index) {
    Program_header ret = {};
    uint8_t BYTE_FORMAT = header.ei_class;
    FILE *fd = fopen(args.path.filepath, "r");
    navigate_fd_to_program_header(header, fd);
    navigate_fd_to_program_index(header, fd, index);

    ret.p_type = read_nbytes_better(header, fd, 4, false);
    if (BYTE_FORMAT == SIXTY_FOUR_BIT) ret.p_flags = read_nbytes_better(header, fd, 4, false);
    ret.p_offset = read_nbytes_better(header, fd, 0, true);
    ret.p_vaddr = read_nbytes_better(header, fd, 0, true);
    ret.p_paddr = read_nbytes_better(header, fd, 0, true);
    ret.p_filesz = read_nbytes_better(header, fd, 0, true);
    ret.p_memsz = read_nbytes_better(header, fd, 0, true);
    if (BYTE_FORMAT == THIRTY_TWO_BIT) ret.p_flags = read_nbytes_better(header, fd, 4, false);
    ret.p_align = read_nbytes_better(header, fd, 0, true);

    return ret;
}
