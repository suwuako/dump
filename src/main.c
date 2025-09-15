#include <stdio.h>

#include "../headers/parse_args.h"

int main(int argc, char *argv[]) {
    printf("hello, dump\n\n");
    Args args = parse_args(argc, argv);
    printf("bool: %d\n", args.dump_header);
    printf("path: %s\n", args.path.filepath);

    return 0;
}
