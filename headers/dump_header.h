#ifndef DUMP_HEADER_H
#define DUMP_HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "parse_args.h"


typedef struct elf_header {
    char magic[4];                // elf magic numbers, 0x7f, 'elf'
    uint8_t class;                // 64/32 bit indicator, 1 for 32, 2 for 64
    uint8_t byteorder;            // endianess, 1 for little, 2 for big
    uint8_t hversion;             // set to 1 for original/current elf version
    uint8_t abi;                  // finds target ABI (0x03 for linux)
    uint8_t abiversion;           // additional abi version
    uint16_t type;                // object file type
    uint16_t isa;                 // target isa
    uint32_t eversion;            // set to 1 for elf version

                                  // these fields are:
                                  // 32 bit if class == 1
                                  // 64 bits if class == 2
    uint64_t entry;               // entry point of where process starts executing
    uint64_t pheader;             // entry point of program header tabl. 
    uint64_t sheader;             // entry point of section header table

    uint32_t flags;               // target architecture changes interpretation of ts
    uint16_t hsize;               // size of elf header (64/52 bytes if 64/32 bits)
    uint16_t phentsize;           // size of program header table entry (0x20 if 32 bit,
                                  // 0x38 if 64 bit)
    uint16_t phnum;               // number of entries in program header table
    uint16_t shentsize;           // size of a section header table entry
                                  // (0x28 if 32 bit, 0x40 if 64 bit)
    uint16_t shnum;               // number of entries in section header table
    uint16_t shent;               // index of section header table entry that contains
                                  // the section names

} Elf_header;


void dump_header(Elf_header header);
Elf_header grab_elf_header(Args args);
void check_and_set_magic(FILE *fd, Elf_header *header);
uint64_t read_varaible_entries(FILE *fd, Elf_header header);

#endif
