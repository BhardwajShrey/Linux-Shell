#include<errno.h>
#include"shell.h"
#include"source.h"


// function to go back one character
void unget_char(struct source_s *src)
{
    if(src->curpos < 0)
    {
        return;
    }

    src->curpos--;
}


// returns next character of the input
char next_char(struct source_s *src)
{
    if(!src || !src->buffer)
    {
        errno = ENODATA;
        return ERRCHAR;
    }

    char c1 = 0;    // c1 will store current character. function will return next character

    if(src->curpos == INIT_SRC_POS)
    {
        src->curpos = -1;
    }
    else
    {
        c1 = src->buffer[src->curpos];
    }

    // this is where pointer is incremented to process next character
    if(++src->curpos >= src->bufsize)
    {
        src->curpos = src->bufsize;
        return EOF;
    }

    return src->buffer[src->curpos];
}


// returns next character but pointer (curpos) is not updated
char peek_char(struct source_s *src)
{
    if(!src || !src->buffer)
    {
        errno = ENODATA;
        return ERRCHAR;
    }

    long pos = src->curpos;

    if(pos == INIT_SRC_POS)
    {
        pos++;
    }

    pos++;

    if(pos >= src->bufsize)
    {
        return EOF;
    }

    return src->buffer[pos];
}


// skip delimiter whitespaces
void skip_white_spaces(struct source_s *src)
{
    char c;

    if(!src || !src->buffer)
    {
        return;
    }

    while(((c = peek_char(src)) != EOF) && (c == ' ' || c == '\t'))
    {
        next_char(src);
    }
}