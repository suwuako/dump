#include <stdio.h>

#include "../headers/dump_section_header.h"
#include "../headers/parse_args.h"
#include "../headers/misc.h"
#include "../headers/dump_elf_header.h"

int main(int argc, char *argv[]) {
    printf("== hello, dump == \n\n");
    Args args = parse_args(argc, argv);

    print_args(args);

    Elf_header elf_header = grab_elf_header(args);
    if (args.dump_header) { dump_header(elf_header); }

    Section_header sect_header = grab_sect_header(elf_header, args);

    DEBUG_DUMP_NBYTES(elf_header.e_shoff + elf_header.e_shentsize, 100, args);

    return 0;
}
