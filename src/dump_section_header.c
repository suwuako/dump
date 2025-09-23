#include <stdio.h>
#include <unistd.h>

#include "../headers/dump_elf_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/misc.h"

uint64_t read_nbytes_better(Elf_header header, FILE *fd, int bytes, bool variable) {
    uint64_t ret = 0;
    if (variable) {

    }

    if (header.class == LITTLE_ENDIAN) {
        for (int i = 0; i < bytes; i++) {
            ret |= (fgetc(fd) << (8 * i));
        }
    } else if (header.class == BIG_ENDIAN) {
        for (int i = 0; i < bytes; i++) {
            ret |= fgetc(fd); ret >>= 8;
        }
    }

    return ret;
}

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

    ret.name = read_nbytes_better(header, fd, 4, false);
    ret.type = read_nbytes_better(header, fd, 4, false);

    printf("name: %ld, type: %ld\n", ret.name, ret.type);


    return ret;
}
