#include <stdio.h>
#include <unistd.h>

#include "../headers/dump_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/misc.h"

void navigate_fd_to_section_header(Elf_header header, FILE *fd) {
    uint64_t entry_point = header.sheader;
    uint64_t entry_count = header.shnum;
    uint64_t entry_size = header.shentsize;
    uint64_t shent_index = header.shent;

    // confirm if shent_index <= entry_conunt;
    if (shent_index > entry_count - 1 ) fatal_error("ERROR: section header index is greater than entry no.");

    lseek(fileno(fd), entry_point, SEEK_SET);

    for (int i = 0; i < shent_index; i++) {
        lseek(fileno(fd), entry_size, SEEK_CUR);
    }
}

Section_header grab_sect_header(Elf_header header, Args args) {
    Section_header ret = {};
    FILE *fd = fopen(args.path.filepath, "r");
    navigate_fd_to_section_header(header, fd);

    return ret;
}
