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

struct word_s
{
    char  *data;    // string representing the word
    int    len;     // length of data
    struct word_s *next;    // pointer to next word. NULL if last word. Linked list
};

struct word_s *make_word(char *str);
void free_all_words(struct word_s *first);  // frees memory used by word structure

#endif