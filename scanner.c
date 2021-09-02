#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shell.h"
#include "scanner.h"
#include "source.h"

char *tok_buf = NULL;   // pointer to buffer where current token is stored
int tok_bufsize  = 0; // no. of bytes allocated to tok_buf
int tok_bufindex = -1;  // current index OR where to add next token

// self explanatory
struct token_s eof_token = 
{
    .text_len = 0,
};

// add single character c to buffer. will extend buffer is size full
void add_to_buf(char c)
{
    tok_buf[tok_bufindex++] = c;

    if(tok_bufindex >= tok_bufsize)
    {
        char *tmp = realloc(tok_buf, tok_bufsize*2);

        if(!tmp)
        {
            errno = ENOMEM;
            return;
        }

        tok_buf = tmp;
        tok_bufsize *= 2;
    }
}

// process string str from input and convert it into structure of type token_s (see scanner.h)
struct token_s *create_token(char *str)
{
    struct token_s *tok = malloc(sizeof(struct token_s));
    
    if(!tok)
    {
        return NULL;
    }

    memset(tok, 0, sizeof(struct token_s));
    tok->text_len = strlen(str);
    
    char *nstr = malloc(tok->text_len + 1); // placeholder for string str
    
    if(!nstr)
    {
        free(tok);
        return NULL;
    }
    
    strcpy(nstr, str);  // placing string str into nstr
    tok->text = nstr;   // adding to struct
    
    return tok;
}

// frees memory used by token structure and then token text
void free_token(struct token_s *tok)
{
    if(tok->text)
    {
        free(tok->text);
    }
    free(tok);
}

// main. this is the real deal. does exactly what it says in your compiler textbook
struct token_s *tokenize(struct source_s *src)
{
    int  endloop = 0;

    // return EOF if src NULL
    if(!src || !src->buffer || !src->bufsize)
    {
        errno = ENODATA;
        return &eof_token;
    }
    
    // allocate space to token buffer
    if(!tok_buf)
    {
        tok_bufsize = 1024;
        tok_buf = malloc(tok_bufsize);
        if(!tok_buf)
        {
            errno = ENOMEM;
            return &eof_token;
        }
    }

    tok_bufindex     = 0;
    tok_buf[0]       = '\0';

    char nc = next_char(src);
    char nc2;
    int i;

    if(nc == ERRCHAR || nc == EOF)
    {
        return &eof_token;
    }

    // loop to read input characters till delimiter or whitespace found. if whitespace found, check token if not-empty.
    // if not-empty, make whitespace a delimiter and break out of loop. else skip
    do
    {
        switch(nc)
        {
            case  '"':
            case '\'':
            case  '`':
                add_to_buf(nc);
                i = find_closing_quote(src->buffer+src->curpos);
                if(!i)
                {
                    src->curpos = src->bufsize;
                    fprintf(stderr, "error: missing closing quote '%c'\n", nc);
                    return &eof_token;
                }
                while(i--)
                {
                    add_to_buf(next_char(src));
                }
                break;

            case '\\':
                nc2 = next_char(src);
                if(nc2 == '\n')
                {
                    break;
                }

                add_to_buf(nc);

                if(nc2 > 0)
                {
                    add_to_buf(nc2);
                }
                break;
                
            case '$':
                add_to_buf(nc);
                nc = peek_char(src);

                if(nc == '{' || nc == '(')
                {
                    i = find_closing_brace(src->buffer+src->curpos+1);
                    if(!i)
                    {
                        src->curpos = src->bufsize;
                        fprintf(stderr, "error: missing closing brace '%c'\n", nc);
                        return &eof_token;
                    }

                    while(i--)
                    {
                        add_to_buf(next_char(src));
                    }
                }
                else if(isalnum(nc) || nc == '*' || nc == '@' || nc == '#' ||
                                       nc == '!' || nc == '?' || nc == '$')
                {
                    add_to_buf(next_char(src));
                }
                break;
            case ' ':
            case '\t':
                if(tok_bufindex > 0)
                {
                    endloop = 1;
                }
                break;
                
            case '\n':
                if(tok_bufindex > 0)
                {
                    unget_char(src);
                }
                else
                {
                    add_to_buf(nc);
                }
                endloop = 1;
                break;
                
            default:
                add_to_buf(nc);
                break;
        }

        if(endloop)
        {
            break;
        }

    } while((nc = next_char(src)) != EOF);

    if(tok_bufindex == 0)
    {
        return &eof_token;
    }
    
    if(tok_bufindex >= tok_bufsize)
    {
        tok_bufindex--;
    }
    tok_buf[tok_bufindex] = '\0';

    struct token_s *tok = create_token(tok_buf); // create a token out of tokbuf

    if(!tok)
    {
        fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
        return &eof_token;
    }

    tok->src = src;
    return tok;
}