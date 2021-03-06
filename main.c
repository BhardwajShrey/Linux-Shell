#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include"shell.h"
#include "source.h"
#include "parser.h"
#include "executor.h"

int main(int argc, char **argv)
{
    char *cmd;

    initsh();
    
    do
    {
        // prompt is characters like > or $ which appear before commands. prompt1 is $
        print_prompt1();

        // read a command. see function after main
        cmd = read_cmd();

        // exit if no command
        if(!cmd)
        {
            exit(EXIT_SUCCESS);
        }

        // null character or enter key pressed without any command
        if(cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
        {
            free(cmd);
            continue;
        }

        // exit if "exit" typed
        if(strcmp(cmd, "exit\n") == 0)
        {
            free(cmd);
            break;
        }

        struct source_s src;
        src.buffer   = cmd;
        src.bufsize  = strlen(cmd);
        src.curpos   = INIT_SRC_POS;
        parse_and_execute(&src);

        free(cmd);

    }while(1);

    exit(EXIT_SUCCESS);
}

char *read_cmd(void)
{
    char buf[1024];
    char *ptr = NULL;
    char ptrlen = 0;

    // commands taken inside 1024 byte buffers. can manually trigger to create another buffer by "\n" after every command

    while(fgets(buf, 1024, stdin))
    {
        int buflen = strlen(buf);

        if(!ptr)
        {
            ptr = malloc(buflen+1);
        }
        else
        {
            char *ptr2 = realloc(ptr, ptrlen+buflen+1);

            if(ptr2)
            {
                ptr = ptr2;
            }
            else
            {
                free(ptr);
                ptr = NULL;
            }
        }

        if(!ptr)
        {
            fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
            return NULL;
        }

        strcpy(ptr+ptrlen, buf);

        if(buf[buflen-1] == '\n')
        {
            if(buflen == 1 || buf[buflen-2] != '\\')
            {
                return ptr;
            }

            ptr[ptrlen+buflen-2] = '\0';
            buflen -= 2;
            print_prompt2();
        }

        ptrlen += buflen;
    }

    return ptr;
}

int parse_and_execute(struct source_s *src)
{
    skip_white_spaces(src);

    struct token_s *tok = tokenize(src);

    if(tok == &eof_token)
    {
        return 0;
    }

    while(tok && tok != &eof_token)
    {
        struct node_s *cmd = parse_simple_command(tok);

        if(!cmd)
        {
            break;
        }

        do_simple_command(cmd);
        free_node_tree(cmd);
        tok = tokenize(src);
    }

    return 1;
}