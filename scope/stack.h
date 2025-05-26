#ifndef STACK_H
#define STACK_H

#include "../symbol table/hashmap.h"

#define MAX_STACK_SIZE 10

typedef struct {
    Table *tables[MAX_STACK_SIZE];
    int size;
    int top;
     
} ScopeStack;

void init_scope_stack(void);
void push_scope(Table *table);
void pop_scope(void);
Table *current_scope(void);
Variable *search_variable(const char *name);



#endif
