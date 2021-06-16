#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"    // includes token_s
#include "source.h"     // includes source_s

struct node_s *parse_simple_command(struct token_s *tok);

#endif