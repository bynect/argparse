#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "argparse.h"

#define eprintf(_fmt, ...)  fprintf(stderr, _fmt, __VA_ARGS__)
#define unused              __attribute__((unused))
#define desc_size           32

static void
mycb(const char *val, unused void *ctx)
{
    if (val != NULL)
        printf("%s\n", val);
    else
    {
        printf(
            "--help, -h      print this message\n"
            "--echo          print its argument\n"
        );
        exit(0);
    }
}

static void
errcb(const char *val, arg_error err, unused void *ctx)
{
    if (val != NULL)
    {
        if (err == ARG_UNKNOWN)
            eprintf("Unknown option: %s\n", val);

        else if (err == ARG_MALFORMED)
            eprintf("Malformed option: %s\n", val);

        else if (err == ARG_MISSING)
            eprintf("Missing argument for option: %s\n", val);
    }
    else
        eprintf("%s\n", "Error");

    exit(1);
}

int
main(int argc, char **argv)
{
    struct arg_description desc[desc_size];
    memset(desc, 0, desc_size * sizeof(struct arg_description));

    desc[0].lname = "help";
    desc[0].sname = "h";
    desc[0].flag = true;
    desc[0].cb = &mycb;

    desc[1].lname = "echo";
    desc[1].flag = false;
    desc[1].cb = &mycb;

    struct args _args;
    init_args(&_args, desc, desc_size, argc, argv, &errcb, NULL);

    return argparse(&_args, false);
}
