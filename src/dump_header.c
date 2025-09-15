#include <stdint.h>
#include <stdio.h>

#include "../headers/misc.h"
#include "../headers/parse_args.h"
#include "../headers/dump_header.h"

// reads 32/64 bits depending of class in elf_header
uint64_t read_varaible_entries(FILE *fd, Elf_header header) {
    uint64_t ret = 0;
    if (header.class == 1) {
        ret = getc(fd);
    } else if (header.class == 2) {
        ret = getc(fd);
        ret <<= 32;
        ret = getc(fd);
    } else {
        fatal_error("ERROR: elf header class is neither 1 nor 2");
    }

    return ret;
}

void check_and_set_magic(FILE *fd, Elf_header *header) {
    header->magic[0] = getc(fd);
    header->magic[1] = getc(fd);
    header->magic[2] = getc(fd);
    header->magic[3] = getc(fd);
}

void dump_header(Args args, Elf_header header) {
}

Elf_header grab_elf_header(Args args) {
    Elf_header ret = {};
    FILE *fd = fopen(args.path.filepath, "r");
    check_and_set_magic(fd, &ret);
    ret.class = getc(fd);
    ret.byteorder = getc(fd);
    ret.hversion = getc(fd);
    ret.abi = getc(fd);
    ret.abiversion = getc(fd);
    ret.type = getc(fd);
    ret.isa = getc(fd);
    ret.eversion = getc(fd);
    ret.entry = read_varaible_entries(fd, ret);
    ret.pheader = read_varaible_entries(fd, ret);
    ret.sheader = read_varaible_entries(fd, ret);
    ret.flags = getc(fd);
    ret.hsize = getc(fd);
    ret.phentsize = getc(fd);
    ret.phnum = getc(fd);
    ret.shentsize = getc(fd);
    ret.shnum = getc(fd);
    ret.shent = getc(fd);

    return ret;
}
