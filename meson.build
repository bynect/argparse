project('c argparse', 'c')

include = include_directories('include')

source = files('src/argparse.c')
example = files('docs/example.c')

lib = library('argparse', source, include_directories : include)
executable('example', example, link_with : lib, include_directories : include)
