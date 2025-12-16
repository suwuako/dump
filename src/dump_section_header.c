#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../headers/dump_elf_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/misc.h"

// bytes indicates how many bytes to read. 0 if is a variable value (32/64 bits)
// factors in little/big endian.
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

    if (header.ei_data == LITTLE_ENDIAN_CUSTOM) {
        for (int i = 0; i < bytes; i++) {
            ret |= (fgetc(fd) << (8 * i));
        }
    } else if (header.ei_data == BIG_ENDIAN_CUSTOM) {
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
    fclose(fd);
}

void navigate_fd_to_section_header(Elf_header header, FILE *fd) {
    uint64_t entry_point = header.e_shoff;

    lseek(fileno(fd), entry_point, SEEK_SET);
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
    fclose(fd);

    return ret;
}

Section_header *grab_all_section_headers(Elf_header header, Args args) {
    Section_header *all_headers = malloc(sizeof(Section_header) * header.e_shnum);

    for (int i = 0; i < header.e_shnum; i++) {
        Section_header section = grab_sect_header(header, args, i);
        all_headers[i] = section;
    }

    return all_headers;
}

void dump_section_headers(Section_header *headers, Elf_header elf_header, Args args) {
    printf("\n== section header dump ==\n\n");
    printf("elf header count: %ld\nshstrntab index: %ld\n\n", elf_header.e_shnum, elf_header.e_shstrndx);

    printf("%s"SH_NAME_ALIGN_STRING SH_TYPE_ALIGN_STRING "    " SH_FLAGS_ALIGN_STRING, "Index", "name", "type", "flags");
    printf("         "SH_FLAGS_ALIGN_STRING "       " SH_FLAGS_ALIGN_STRING "         " SH_FLAGS_ALIGN_STRING"", "addr", "offset", "size");
    printf("     "SH_FLAGS_ALIGN_STRING "     " SH_FLAGS_ALIGN_STRING "        "SH_FLAGS_ALIGN_STRING "      " SH_FLAGS_ALIGN_STRING"\n", "link", "info", "align", "entsize");
    for (int i = 0; i < elf_header.e_shnum; i++) {
        print_and_format_section_header(headers[elf_header.e_shstrndx], headers[i], elf_header, i, args);
    }
}

void print_sh_type_entry(uint64_t value) {
    const char *result = NULL;
    switch (value) {
        case 0x0:        result = "SHT_NULL"; break;
        case 0x1:        result = "SHT_PROGBITS"; break;
        case 0x2:        result = "SHT_SYMTAB"; break;
        case 0x3:        result = "SHT_STRTAB"; break;
        case 0x4:        result = "SHT_RELA"; break;
        case 0x5:        result = "SHT_HASH"; break;
        case 0x6:        result = "SHT_DYNAMIC"; break;
        case 0x7:        result = "SHT_NOTE"; break;
        case 0x8:        result = "SHT_NOBITS"; break;
        case 0x9:        result = "SHT_REL"; break;
        case 0x0A:       result = "SHT_SHLIB"; break;
        case 0x0B:       result = "SHT_DYNSYM"; break;
        case 0x0E:       result = "SHT_INIT_ARRAY"; break;
        case 0x0F:       result = "SHT_FINI_ARRAY"; break;
        case 0x10:       result = "SHT_PREINIT_ARRAY"; break;
        case 0x11:       result = "SHT_GROUP"; break;
        case 0x12:       result = "SHT_SYMTAB_SHNDX"; break;
        case 0x13:       result = "SHT_NUM"; break;
        case 0x60000000: result = "SHT_LOOS"; break;
        default:         result = "UNKNOWN"; break;
    }

    printf(SH_TYPE_ALIGN_STRING, result);
}

void read_stream_until_null(FILE *fd) {
    char result[100];
    int i = 0;

    // read string until null term
    while ((result[i] = fgetc(fd)) != '\0') i++;

    printf(SH_NAME_ALIGN_STRING, result);
}


void print_and_format_section_header(Section_header shname, Section_header h, Elf_header elf_header, int i, Args args) {
    int addr = shname.sh_offset + h.sh_name;
    FILE *fd = fopen(args.path.filepath, "r");
    lseek(fileno(fd), addr, SEEK_SET);

    printf("[%2d] ", i);
    read_stream_until_null(fd);
    print_sh_type_entry(h.sh_type);
    printf(SH_FLAGS_ALIGN_STRING "0x%04lx", "", h.sh_flags);
    printf(SH_FLAGS_ALIGN_STRING "0x%08lx", "", h.sh_addr);
    printf(SH_FLAGS_ALIGN_STRING "0x%08lx", "", h.sh_offset);
    printf(SH_FLAGS_ALIGN_STRING "0x%08lx", "", h.sh_size);
    printf(SH_FLAGS_ALIGN_STRING "0x%04lx", "", h.sh_link);
    printf(SH_FLAGS_ALIGN_STRING "0x%04lx", "", h.sh_info);
    printf(SH_FLAGS_ALIGN_STRING "0x%08lx", "", h.sh_addralign);
    printf(SH_FLAGS_ALIGN_STRING "0x%08lx", "", h.sh_entsize);

    printf("\n");


    fclose(fd);
}
