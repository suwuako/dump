#include <stdint.h>
#include <stdio.h>

#include "../headers/misc.h"
#include "../headers/parse_args.h"
#include "../headers/dump_header.h"

const char *osabi_names[19] = {
  "SystemV", "HP_UX", "NetBSD", "Linux", "GNU_Hurd", "Solaris", "AIX",
  "IRIX", "FreeBSD", "Tru64", "Novell_Modesto", "OpenBSD", "OpenVMS",
  "NonStop_Kernel", "AROS", "FenixOS", "Nuxi_CloudABI", "Stratus_Technologies_OpenVOS"
};

const char *object_type_names[5] = {
    "Unknown", "Relocatable", "Executable", "Shared Object", "Core"
};

/*
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
*/

// IM SO UGLY 😭 sigh... 💔
// is goto aura?
uint64_t read_nbytes(FILE *fd, Elf_header *header, int byte_count, bool variable) {
    uint64_t ret = 0;

    if (variable) {
        if (header->class == 1) {
            byte_count = 4;
        } else {
            byte_count = 8;
        }
    }

    // little endian
    if (header->byteorder == 1) {
        ret |= getc(fd);
        ret |= ((uint64_t) getc(fd) << (8));
        if (byte_count == 2) { goto READ_NBYTES__END; }
        
        ret |= ((uint64_t) getc(fd) << (8 * 2));
        ret |= ((uint64_t) getc(fd) << (8 * 3));
        if (byte_count == 4) { goto READ_NBYTES__END; }
        
        ret |= ((uint64_t) getc(fd) << (8 * 4));
        ret |= ((uint64_t) getc(fd) << (8 * 5));
        ret |= ((uint64_t) getc(fd) << (8 * 6));
        ret |= ((uint64_t) getc(fd) << (8 * 7));
        if (byte_count == 8) { goto READ_NBYTES__END; }
    } else if (header->byteorder == 2) {
        ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        if (byte_count == 4) { goto READ_NBYTES__END; }

        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        if (byte_count == 4) { goto READ_NBYTES__END; }

        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        ret <<= 8; ret |= getc(fd);
        if (byte_count == 8) { goto READ_NBYTES__END; }
    } else {
        fatal_error("ERROR: expected endianess to be 1 or 2.");
    }

READ_NBYTES__END:
    return ret;
}


void check_and_set_magic(FILE *fd, Elf_header *header) {
    header->magic[0] = getc(fd);
    header->magic[1] = getc(fd);
    header->magic[2] = getc(fd);
    header->magic[3] = getc(fd);
}

void dump_header(Elf_header header) {
    printf("\n== elf header dump ==\n");
    printf("\tMAGIC:                            \t%x, %s\n", header.magic[0], &header.magic[1]);
    printf("\tformat:                           \t%d bits\n", (header.class == 1) ? 32 : 64);
    printf("\tbyte order:                       \t%s endian\n", (header.byteorder == 1) ? "little" : "big");
    printf("\tei_ver:                           \t%d (should be 1)\n", header.hversion);
    printf("\tosabi:                            \t%s\n", osabi_names[header.abi]);
    printf("\tabiversion+:                      \t%d\n", header.abiversion);
    printf("\ttype:                             \t%s\n", (header.type <= 0x04) ? object_type_names[header.type] : "Reserved Other");
    printf("\ttarget ISA:                       \t%s, %ld\n", (header.isa) == 0x3e ? "AMD x86-64" : "not amd x86", header.isa);
    printf("\teversion:                         \t0x%lx\n", header.eversion);
    printf("\tprog entry address:               \t0x%lx\n", header.entry);
    printf("\tprog header address:              \t%ld (bytes into file)\n", header.pheader);
    printf("\tsection header offset:            \t%ld (bytes into file)\n", header.sheader);
    printf("\tflags:                            \t0x%ld\n", header.flags);
    printf("\theader size:                      \t%ld (bytes)\n", header.hsize);
    printf("\tprogram header entry size:        \t%ld (bytes)\n", header.phentsize);
    printf("\tprogram header entry num:         \t%ld\n", header.phnum);
    printf("\tsection header entry size:        \t%ld (bytes)\n", header.shentsize);
    printf("\tsection header entry num:         \t%ld\n", header.shnum);
    printf("\tsection header string table index:\t%ld\n", header.shent);
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
    for (int i = 0; i < PADDING_SIZE; i++) {
        ret.padding[i] = getc(fd);
    }
    ret.type = read_nbytes(fd, &ret, 2, false);
    ret.isa = read_nbytes(fd, &ret, 2, false);
    ret.eversion = read_nbytes(fd, &ret, 4, false);
    ret.entry = read_nbytes(fd, &ret, 0, true);
    ret.pheader = read_nbytes(fd, &ret, 0, true);
    ret.sheader = read_nbytes(fd, &ret, 0, true);
    ret.flags = read_nbytes(fd, &ret, 4, false);
    ret.hsize = read_nbytes(fd, &ret, 2, false);
    ret.phentsize = read_nbytes(fd, &ret, 2, false);
    ret.phnum = read_nbytes(fd, &ret, 2, false);
    ret.shentsize = read_nbytes(fd, &ret, 2, false);
    ret.shnum = read_nbytes(fd, &ret, 2, false);
    ret.shent = read_nbytes(fd, &ret, 2, false);

    return ret;
}
