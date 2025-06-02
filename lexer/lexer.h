#ifndef LEXER_H_
#define LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <ctype.h>



typedef enum {
   BEGINNING,
   INT,
   KEYWORD,
   SEPARATOR,
   OPERATOR,
   IDENTIFIER,
   STRING,
   COMMENT,
   UNKNOWN,
   END_OF_TOKENS,
 } TokenType;
 
 typedef struct {
   TokenType type;
   char *value;
   size_t line_num;
 } Token;
 






Token *lexer(FILE *);
const char *token_type_to_string(TokenType);
void print_tokens(Token *);
#endif