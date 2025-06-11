#ifndef PARSER_H_
#define PARSER_H_
#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <assert.h>
#include <string.h>
#include "../symbol_table/hashmap.h"
#include "../scope/stack.h"
#include "../lexer/lexer.h"



typedef struct Node{
   char *value;
   TokenType type;
   struct Node *left;
   struct Node *right;
 } Node;


Node *createNode(char *);
Node *postfix_prefix(Node *,Token **);
Node *parser(Token *);
void handle_exit_system(Node *,Token **);
Node *function_call(Node *,Node *,Token **);
Node *parse_expression(Token **);
Node *parse_primary(Token **);
Node *parse_multiplication(Token **);
size_t size_of_type(const char *type);
Node *create_variables(Node *,Token **);
Node *if_statement_generation(Node *,Token **);
Node *while_statement_generation(Node *node,Token **);
Node *handle_variable_reassignment(Node *,Token **);
void handle_comments(Token **);


#endif