#ifndef DUMP_HEADER_H
#define DUMP_HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "parse_args.h"

#define THIRTY_TWO_BIT 1
#define SIXTY_FOUR_BIT 2
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 2
#define PADDING_SIZE 7

extern const char *osabi_names[19];
extern const char *object_type_names[5];

typedef struct elf_header {
    char magic[4];                // elf magic numbers, 0x7f, 'elf'
    uint8_t class;                // 64/32 bit indicator, 1 for 32, 2 for 64
    uint8_t byteorder;            // endianess, 1 for little, 2 for big
    uint8_t hversion;             // set to 1 for original/current elf version
    uint8_t abi;                  // finds target ABI (0x03 for linux)
    uint8_t abiversion;           // additional abi version
    uint8_t padding[PADDING_SIZE];           // padding bytes
    uint64_t type;                // object file type
    uint64_t isa;                 // target isa
    uint64_t eversion;            // set to 1 for elf version

                                  // these fields are:
                                  // 32 bit if class == 1
                                  // 64 bits if class == 2
    uint64_t entry;               // entry point of where process starts executing
    uint64_t pheader;             // entry point of program header tabl. 
    uint64_t sheader;             // entry point of section header table

    uint64_t flags;               // target architecture changes interpretation of ts
    uint64_t hsize;               // size of elf header (64/52 bytes if 64/32 bits)
    uint64_t phentsize;           // size of program header table entry (0x20 if 32 bit,
                                  // 0x38 if 64 bit)
    uint64_t phnum;               // number of entries in program header table
    uint64_t shentsize;           // size of a section header table entry
                                  // (0x28 if 32 bit, 0x40 if 64 bit)
    uint64_t shnum;               // number of entries in section header table
    uint64_t shent;               // index of section header table entry that contains
                                  // the section names

} Elf_header;


void dump_header(Elf_header header);
Elf_header grab_elf_header(Args args);
void check_and_set_magic(FILE *fd, Elf_header *header);
uint64_t read_varaible_entries(FILE *fd, Elf_header header);
uint64_t read_nbytes(FILE *fd, Elf_header *header, int byte_count, bool variable);

#endif
