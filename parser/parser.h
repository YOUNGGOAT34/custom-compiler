#ifndef PARSER_H_
#define PARSER_H_
#include<stdio.h>
#include "../lexer/lexer.h"
#include "parser.c"





Node *createNode(char *);
Node *parser(Token *);

void handle_exit_system(Node *current,Token **);
Node *parse_expression(Token **);
Node *parse_primary(Token **);
Node *parse_multiplication(Token **);
void preorderTraversal(Node*);

#endif