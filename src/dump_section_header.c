#include <stdio.h>
#include <stdlib.h>
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
    lseek(fileno(fd), index * header.e_shentsize, SEEK_CUR);
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

Section_header grab_sect_header(Elf_header header, Args args, int index) {
    Section_header ret = {};
    FILE *fd = fopen(args.path.filepath, "r");
    navigate_fd_to_section_header(header, fd);
    navigate_fd_to_section_index(header, fd, index);

    ret.sh_name = read_nbytes_better(header, fd, 4, false);
    ret.sh_type = read_nbytes_better(header, fd, 4, false);
    ret.sh_flags = read_nbytes_better(header, fd, 0, true);
    ret.sh_addr = read_nbytes_better(header, fd, 0, true);
    ret.sh_offset = read_nbytes_better(header, fd, 0, true);
    ret.sh_size = read_nbytes_better(header, fd, 0, true);
    ret.sh_link = read_nbytes_better(header, fd, 4, false);
    ret.sh_info = read_nbytes_better(header, fd, 4, false);
    ret.sh_addralign = read_nbytes_better(header, fd, 0, true);
    ret.sh_entsize = read_nbytes_better(header, fd, 0, true);

    return ret;
}

Section_header *grab_all_section_headers(Elf_header header, Args args) {
    Section_header *all_headers = malloc(sizeof(Section_header) * header.e_shnum);

    for (int i = 0; i < header.e_shnum; i++) {
        Section_header section = grab_sect_header(header, args, i);
        all_headers[i] = section;
    }
}

void dump_section_headers(Section_header *headers, Elf_header elf_header) {
    for (int i = 0; i < elf_header.e_shnum; i++) {
        print_and_format_section_header(headers[elf_header.e_shstrndx], headers[i], elf_header, i);
    }
}

void print_and_format_section_header(Section_header shname, Section_header h, Elf_header elf_header, int i) {
    // read string until null term
}
