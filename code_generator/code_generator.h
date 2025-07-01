#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include<stdio.h>

#include<stdlib.h>
#include "../parser/parser.h"
#include "../symbol_table/hashmap.h"
#include "../scope/stack.h"

void preoder_traversal(Node *,FILE *);
void postorder_traversal(Node *, FILE *,bool,Returntype);
void generate_data_section(Node *,FILE *);
void write_to_console(Node *,FILE *);
void code_generator(Node *);
void if_statement(Node *,FILE *);

#endif