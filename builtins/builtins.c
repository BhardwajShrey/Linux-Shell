#include "../shell.h"

struct builtin_s builtins[] =
{   
    { "dump", dump},    // print contents of symbol table
};

int builtins_count = sizeof(builtins)/sizeof(struct builtin_s);