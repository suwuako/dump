// this is a test library for me to figure out how the elf format works
// in this case, the "program header" for an elf file is how the OS knows how to load an
// elf executable binary. What happens if i instead load the program header on a static
// lib? this is the static lib in squestion

// ANSWER: 
//  program header entry size:        	0 (bytes)
//  program header entry num:         	0
int sum(int a, int b) {
    return a + b;
}
