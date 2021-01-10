#include <stdio.h>
#include <string.h>

#include "argparse.h"

#define eprintf(_fmt, ...)  fprintf(stderr, _fmt, __VA_ARGS__)
#define desc_size 32

static void
mycb(const char *val)
{
    if (val != NULL)
        printf("%s\n", val);
    else
        printf(
            "--help, -h      print this message\n"
            "--echo          print its argument\n"
        );
}

static void
errcb(const char *val, arg_error err)
{
    if (val != NULL && err == ARG_UNKNOWN)
        eprintf("Unknown option: %s\n", val);
    else
        eprintf("Error: %s\n", val);
}

int
main(int argc, char **argv)
{
    struct args_description desc[desc_size];
    memset(desc, 0, desc_size * sizeof(struct args_description));

    desc[0].lname = "help";
    desc[0].sname = "h";
    desc[0].flag = true;
    desc[0].cb = &mycb;

    desc[1].lname = "echo";
    desc[1].flag = false;
    desc[1].cb = &mycb;

    struct args _args;
    init_args(&_args, desc, desc_size, argc, argv, &errcb);

    return argparse(&_args, false);
}
