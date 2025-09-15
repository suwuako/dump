#include <stdio.h>

#include "../headers/parse_args.h"
#include "../headers/misc.h"
#include "../headers/dump_header.h"

int main(int argc, char *argv[]) {
    printf("hello, dump\n\n");
    Args args = parse_args(argc, argv);

    print_args(args);

    if (args.dump_header) { dump_header(args); }

    return 0;
}
