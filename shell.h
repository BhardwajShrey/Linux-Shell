#ifndef SHELL_H
#define SHELL_H

void print_prompt1(void);
void print_prompt2(void);

char *read_cmd(void);

void initsh(void);

#include "source.h"
int  parse_and_execute(struct source_s *src);

// shell built-in utilities
int dump(int argc, char **argv);    // dump function prototype

// define built-in utilities
struct builtin_s
{
    char *name;     // name
    int (*func)(int argc, char **argv); // pointer to function to execute utility
};

// list of built-in utilities
extern struct builtin_s builtins[];

// count of utilities
extern int builtins_count;

#endif