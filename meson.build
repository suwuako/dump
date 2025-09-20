project('dump', 'c')

deps = [
    'src/misc.c', 'src/parse_args.c',
    'src/dump_header.c', 'src/dump_section_header.c'
    ]

executable('dump', 'src/main.c', deps)


run_target('run',
    command : 'scripts/run.sh')
