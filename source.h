#ifndef SOURCE_H
#define SOURCE_H

#define EOF             (-1)
#define ERRCHAR         ( 0)

#define INIT_SRC_POS    (-2)

struct source_s
{
    char *buffer; // input text
    long bufsize; // size of input text
    long  curpos; // character position in source (absolute). where to pick next char from
};

char next_char(struct source_s *src);
void unget_char(struct source_s *src);
char peek_char(struct source_s *src);
void skip_white_spaces(struct source_s *src);

#endif