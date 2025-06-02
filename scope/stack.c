#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

ScopeStack scope_stack;
ScopeStack code_gen_stack;

void init_scope_stack(ScopeStack *stack) {
    stack->top = -1;
    stack->size = 0;
}


void push_scope(ScopeStack *stack,Table *table) {
    if (!table){
         printf("Trying to push a null table\n");
         return ;
    }
    if (stack->top >= MAX_STACK_SIZE - 1) {
        printf("Scope stack overflow\n");
        exit(1);
    }
    stack->tables[++stack->top] = table;
   
    stack->size++;
   
}

void pop_scope(ScopeStack *stack) {
    if (stack->top < 0) {
        printf("Scope stack underflow\n");
        exit(1);
    }
    // printf("Popping from stack: %s\n", stack == &scope_stack ? "Parsing Stack" : "CodeGen Stack");

    clear_hashmap(stack->tables[stack->top]->map);
    stack->top--;
}

Table *current_scope(ScopeStack *stack) {
    if (stack->top < 0) {
        printf("No active scope\n");
        exit(1);
    }
    return stack->tables[stack->top];
}

Variable *search_variable(ScopeStack *stack,const char *name) {
    
    for (int i = stack->top; i >= 0; i--) {
        Table *table=stack->tables[i];
        if(table==NULL){
         
          continue;
        }
        Variable *var = hashmap_get(stack->tables[i]->map, name);
       
        if (var != NULL) {
            
            return var;
        }
       
    }
    
    
    return NULL;
}


//will be useful in code generation
void reverse_codegen_stack(ScopeStack *stack) {
    ScopeStack temp_stack;
    init_scope_stack(&temp_stack);  

    while (stack->top >= 0) {
        push_scope(&temp_stack, stack->tables[stack->top]);  
        stack->top--;  
    }

    *stack = temp_stack; 
}