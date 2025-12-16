#include <stdbool.h>
#include "../headers/dump_program_header.h"
#include "../headers/dump_elf_header.h"

// e_phnum Contains the number of entries in the program header table. 
// if e_phnum is 0, program header doesn't exist
bool program_header_exists(Elf_header header) {
    if (header.e_phnum == 0) return false;
    return true;
}
