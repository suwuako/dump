#include <stdio.h>
#include <stdlib.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"
#include "../headers/dump_elf_header.h"
#include "../headers/dump_section_header.h"
#include "../headers/dump_program_header.h"

int main(int argc, char *argv[]) {
    printf("== hello, dump == \n\n");
    Args args = parse_args(argc, argv);

    print_args(args);

    Elf_header elf_header = grab_elf_header(args);
    if (args.dump_header) dump_header(elf_header);

    Section_header *section_headers = grab_all_section_headers(elf_header, args);
    if (args.dump_section_header) dump_section_headers(section_headers, elf_header, args);

    if (program_header_exists(elf_header)) printf("meow");

    free(section_headers);

    return 0;
}
