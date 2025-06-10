#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include<stdio.h>

#include<stdlib.h>
#include "../parser/parser.h"
#include "../symbol_table/hashmap.h"
#include "../scope/stack.h"

void traverse(Node *root, FILE *file);
void generate_data_section(Node *root,FILE *file);
void code_generator(Node *root);
void if_statement(Node *root,FILE *file);

#endif