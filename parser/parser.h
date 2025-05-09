#ifndef PARSER_H_
#define PARSER_H_
#include<stdio.h>
#include "../lexer/lexer.h"
#include "parser.c"





Node *createNode(char *val);
Node *parser(Token *tokens);
void handle_exit_system(Node *current,Token **current_token);
Node *parse_expression(Token **current_token_ptr);
Node *parse_primary(Token **current_token_ptr);
Node *parse_multiplication(Token **current_token_ptr);
void preorderTraversal(Node* root);

#endif