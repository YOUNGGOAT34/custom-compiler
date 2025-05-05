#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include<stdio.h>
#include "../parser/parser.h"
#include "code_generator.c"

void generate_data_section(Node *root,FILE *file);
void code_generator(Node *root);

#endif