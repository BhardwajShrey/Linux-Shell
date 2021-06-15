#include<stdio.h>
#include"shell.h"

// prompts before every line...

void print_prompt1(void)
{
    fprintf(stderr, "$ ");
}

void print_prompt2(void)
{
    fprintf(stderr, "> ");
}