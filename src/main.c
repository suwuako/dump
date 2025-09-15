#include <stdio.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"
#include "../headers/dump_header.h"

int main(int argc, char *argv[]) {
    printf("hello, dump\n\n");
    Args args = parse_args(argc, argv);

    print_args(args);

    Elf_header elf_header = grab_elf_header(args);
    if (args.dump_header) { dump_header(args, elf_header); }

    return 0;
}
