#ifndef STACK_H
#define STACK_H

#include "../symbol_table/hashmap.h"

#define MAX_STACK_SIZE 10



typedef struct {
    Table *tables[MAX_STACK_SIZE];
    int size;
    int top;
     
} ScopeStack;

/*I need one stack for just parsing ,,and the other one for code generation
  So basically the two stacks will be Identical ,difference is ,the code generation stack won't be popped in the parsing stage 
  In the parsing stage I will pop only the Scope stack ,and later during code generation I will reverse the code generation stack 
  since it will be in reverse
*/

extern ScopeStack scope_stack;
extern ScopeStack code_gen_stack;




void init_scope_stack(ScopeStack *);
void push_scope(ScopeStack *,Table *);
void pop_scope(ScopeStack *);
Table *current_scope(ScopeStack *);
Variable *search_variable(ScopeStack *,const char *);
void reverse_codegen_stack(ScopeStack *);


#endif
