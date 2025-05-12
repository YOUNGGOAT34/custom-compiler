#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

ScopeStack scope_stack;

void init_scope_stack() {
    scope_stack.top = -1;
}

void push_scope(Table *table) {
    if (!table){
         printf("Trying to push a null table\n");
         return ;
    }
    if (scope_stack.top >= MAX_STACK_SIZE - 1) {
        printf("Scope stack overflow\n");
        exit(1);
    }
    scope_stack.tables[++scope_stack.top] = table;
    scope_stack.size++;
}

void pop_scope() {
    if (scope_stack.top < 0) {
        printf("Scope stack underflow\n");
        exit(1);
    }
    clear_hashmap(scope_stack.tables[scope_stack.top]->map);
    scope_stack.top--;
}

Table *current_scope() {
    if (scope_stack.top < 0) {
        printf("No active scope\n");
        exit(1);
    }
    return scope_stack.tables[scope_stack.top];
}

Variable *search_variable(const char *name) {
    for (int i = scope_stack.top; i >= 0; i--) {
        Table *table=scope_stack.tables[i];
        if(table==NULL){
         
          continue;
        }
        Variable *var = hashmap_get(scope_stack.tables[i]->map, name);
        
        if (var != NULL) return var;
        
    }
    
   
    return NULL;
}
