#ifndef PARSER_H_
#define PARSER_H_
#include<stdio.h>
#include "../lexer/lexer.h"
#include "parser.c"



  
Node *createNode(char *val);
int get_precedence(char *operation);
Node *parser(Token *tokens);
Node *parse_expression(Token **current_token_ptr);
Node *parse_primary(Token **current_token_ptr);
Node *parse_multiplication(Token **current_token_ptr);
void preorderTraversal(Node* root);

#endif