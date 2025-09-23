#include <stdio.h>
#include <unistd.h>

#include "../headers/dump_elf_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/misc.h"

uint64_t read_nbytes_better(Elf_header header, FILE *fd, int bytes, bool variable) {
    uint64_t ret = 0;
    if (variable) {
        if (header.ei_class == THIRTY_TWO_BIT) {
            // if 32 bit
            bytes = VARIABLE_32BIT_SIZE;
        } else {
            bytes = VARIABLE_64BIT_SIZE;
        }
    }

    if (header.ei_class == LITTLE_ENDIAN) {
        for (int i = 0; i < bytes; i++) {
            ret |= (fgetc(fd) << (8 * i));
        }
    } else if (header.ei_class == BIG_ENDIAN) {
        for (int i = 0; i < bytes; i++) {
            ret |= fgetc(fd); ret >>= 8;
        }
    }

    return ret;
}

void navigate_fd_to_section_index(Elf_header header, FILE *fd, int index) {

}

// debug function to dump nbytes from a file offset.
void DEBUG_DUMP_NBYTES(int offset, int n, Args args) {
    FILE *fd = fopen(args.path.filepath, "r");

    lseek(fileno(fd), offset, SEEK_SET);
    for (int i = 0 ; i < n; i++) {
        if (i % 16 == 0) {
            printf("\n");
        } if (i % 2 == 0) {
            printf(" ");
        }
        printf("%02x", fgetc(fd));
    }
}

void navigate_fd_to_section_header(Elf_header header, FILE *fd) {
    uint64_t entry_point = header.e_shoff;
    uint64_t entry_count = header.e_shnum;
    uint64_t entry_size = header.e_shentsize;
    uint64_t shent_index = header.e_shstrndx;

    // confirm if shent_index <= entry_conunt;
    // if (shent_index > entry_count - 1 ) fatal_error("ERROR: section header index is greater than entry no.");

    lseek(fileno(fd), entry_point + entry_size, SEEK_SET);
}

Section_header grab_sect_header(Elf_header header, Args args) {
    Section_header ret = {};
    FILE *fd = fopen(args.path.filepath, "r");
    navigate_fd_to_section_header(header, fd);

    ret.sh_name = read_nbytes_better(header, fd, 4, false);
    ret.sh_type = read_nbytes_better(header, fd, 4, false);

    printf("name: %ld, type: %ld\n", ret.sh_name, ret.sh_type);


    return ret;
}
