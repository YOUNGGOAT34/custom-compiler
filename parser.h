#ifndef PARSER_H_
#define PARSER_H_
#include<stdio.h>
#include "lexer.h"
typedef struct Node{
   char *data;
   struct Node *left;
    struct Node *right;
}Node;

Node *createNode(char *val);
void parser(Token *tokens);
void preorderTraversal(Node* root);

#endif