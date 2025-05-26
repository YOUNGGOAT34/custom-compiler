#ifndef LEXER_H_
#define LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "lexer.c"


Token *lexer(FILE *);
const char *token_type_to_string(TokenType);
void print_tokens(Token *);
#endif