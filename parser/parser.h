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


// Node *createNode(char *);
// void print__variables(void) ;
Variable *search_global( const char *);
Node *postfix_prefix(Node *,Token **);
bool check_variable(char *);
Returntype return_type_to_integer(Token **,char *);
Node *parser(Token *);
void handle_exit_system(Node *,Token **);
Node *function_call(Node *,Node *,Token **);
Node *parse_expression(Token **);
Node *parse_primary(Token **);
Node *parse_multiplication(Token **);
size_t size_of_type(Returntype);
Node *create_variables(Node *,Token **,bool);
Node *if_statement_generation(Node *,Token **);
Node *while_statement_generation(Node *node,Token **);
Node *arrays(Node *,Token **);
Node *handle_variable_reassignment(Node *,Token **);
Node *handle_writing_to_the_console(Node *,Token **);



#endif