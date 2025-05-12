#ifndef STACK_H
#define STACK_H

#include "../symbol table/hashmap.h"

#define MAX_STACK_SIZE 10

typedef struct {
    Table *tables[MAX_STACK_SIZE];
    int size;
    int top;
     
} ScopeStack;

void init_scope_stack();
void push_scope(Table *table);
void pop_scope();
Table *current_scope();
Variable *search_variable(const char *name);

extern ScopeStack scope_stack;

#endif
