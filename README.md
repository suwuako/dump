# dump

my implementation of `objdump` + `readelf -h`. A precursor to my linker.


## Compilation

This project uses `meson`. Ensure dependencies are installed using your package manager
of choice.

- meson
- ninja
- python3

First, setup build directory

`$ meson setup build`

Compile the project with meson.

`$ meson compile -C build`

Test if the binary functions by dumping the contents of itself.

`/dump/build/ $: ./dump -h dump`

Optionally, you can run dump after ninja successfully compiles the project:

`$ meson compile -C build run`

## Usage
```
Usage: dump [OPTION]... FILE
Dumps compiled binaries headers, symbols, etc.

    -h, --dump-header           dump ELF headers to stdout
    -s, --dump-section-header   dump section headers to stdout
    -p, --dump-program-header   dump program headers to stdout

Examples:
    dump -h a.out       dumps the ELF headers from a.out
```
