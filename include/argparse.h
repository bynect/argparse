#ifndef _ARGPARSE_H
#define _ARGPARSE_H

#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    ARG_SUCCESS,
    ARG_MALFORMED,
    ARG_MISSING,
    ARG_UNKNOWN,
} arg_error;

typedef void (*arg_cb)(const char *val);
typedef void (*error_cb)(const char *val, arg_error err);

struct args_description
{
    char *lname;
    char *sname;
    int flag;
    arg_cb cb;
};

struct args
{
    struct args_description *desc;
    unsigned int desc_len;
    char **argv;
    int argc;
    error_cb err_cb;
};

void init_args(struct args *_args, struct args_description *desc,
            int desc_len, int argc, char **argv, error_cb err_cb);

arg_error argparse(struct args *_args, bool fail_fast);

#endif /*_ARGPARSE_H*/