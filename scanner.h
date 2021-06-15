#ifndef SCANNER_H
#define SCANNER_H

struct token_s
{
    struct source_s *src; // source of input. refer source.h and source.c
    int text_len; // length of token
    char *text; // actual token
};

// EOF token
extern struct token_s eof_token;

struct token_s *tokenize(struct source_s *src);
void free_token(struct token_s *tok);

#endif