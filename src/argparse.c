#include <stdio.h>
#include <string.h>

#include "argparse.h"

#define call_cb(_cb, ...)   if (_cb != NULL) _cb(__VA_ARGS__)

void
init_args(struct args *_args, struct arg_description *desc,
        int desc_len, int argc, char **argv, error_cb err_cb, void *ctx)
{
    _args->desc = desc;
    _args->desc_len = desc_len;
    _args->argc = argc;
    _args->argv = argv;
    _args->err_cb = err_cb;
    _args->ctx = ctx;
}

arg_error
argparse(struct args *_args, bool fail_fast)
{
    int i, j;
    bool known, single;
    arg_error ret = ARG_SUCCESS;

    for (i = 0; i < _args->argc; ++i)
    {
        char *arg = _args->argv[i];
        const int len = strlen(arg);
        unsigned int offset = 1;

        if (arg == NULL) break;

        single = true;
        if (arg[0] == '-')
        {
            if (arg[1] == '-')
            {
                if (len == 2) break;
                offset = 2;
                single = false;
            }

            known = false;
            for (j = 0; j < _args->desc_len; ++j)
            {
                const char *desc_entry = single ? _args->desc[j].sname : _args->desc[j].lname;
                if (desc_entry == NULL) continue;
                const int desc_len = strlen(desc_entry);

                if (!strncmp(arg + offset, desc_entry, desc_len))
                {
                    char *val = NULL;
                    known = true;

                    if (len - offset == desc_len)
                    {
                        if (!_args->desc[j].flag)
                        {
                            if (++i > _args->argc)
                            {
                                call_cb(_args->err_cb, arg, ARG_MISSING, _args->ctx);

                                if (fail_fast)
                                    return ARG_MISSING;
                                else
                                {
                                    ret = ARG_MISSING;
                                    goto _continue;
                                }
                            }

                            if ((val = _args->argv[i]) == NULL)
                            {
                                call_cb(_args->err_cb, arg, ARG_MISSING, _args->ctx);

                                if (fail_fast)
                                    return ARG_MISSING;
                                else
                                {
                                    ret = ARG_MISSING;
                                    goto _continue;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (!_args->desc[j].flag)
                        {
                            if (offset + desc_len > len)
                            {
                                call_cb(_args->err_cb, arg, ARG_MALFORMED, _args->ctx);

                                if (fail_fast)
                                    return ARG_MALFORMED;
                                else
                                {
                                    ret = ARG_MALFORMED;
                                    goto _continue;
                                }
                            }

                            if (*(arg + offset + desc_len) == '=' && !single) ++offset;
                            val = arg + offset + desc_len;
                        }
                        else if (len - offset != desc_len)
                        {
                            known = false;
                            break;
                        }
                    }

                    call_cb(_args->desc[j].cb, val, _args->ctx);
                    break;
                }
            }

            if (!known)
            {
                call_cb(_args->err_cb, arg, ARG_UNKNOWN, _args->ctx);

                if (fail_fast)
                    return ARG_UNKNOWN;
                else
                    ret = ARG_UNKNOWN;
            }

        _continue: ;
        }
    }

    return ret;
}
