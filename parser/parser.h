#ifndef PARSER_H_
#define PARSER_H_
#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <assert.h>
#include <string.h>
#include "../symbol table/hashmap.h"
#include "../scope/stack.h"
#include "../lexer/lexer.h"



typedef struct Node{
   char *value;
   TokenType type;
   struct Node *left;
   struct Node *right;
 } Node;


Node *createNode(char *);
Node *parser(Token *);
void handle_exit_system(Node *current,Token **);
Node *parse_expression(Token **);
Node *parse_primary(Token **);
Node *parse_multiplication(Token **);

Node *create_variables(Node *current,Token **current_token_ptr);
Node *if_statement_generation(Node *node,Token **current_token_ptr);
Node *while_statement_generation(Node *node,Token **);
Node *handle_variable_reassignment(Node *node,Token **current_token_ptr);


#endif