
#include "code_generator.h"


static int if_label_counter=0;
static int while_label_counter=0;
static int do_while_label_counter=0;

//while statement code generation
void while_statement(Node *root,FILE *file){
    char start_loop[64],end_loop [64];
    int label_counter=while_label_counter++;
    sprintf(start_loop,"while%d",label_counter);
    sprintf(end_loop,"end_while%d",label_counter);
    //get the code block,condition block
    Node *condition=root->left->left;
    Node *code_block=root->left->right->left;
    //we also need the lhs and rhs of the condition
    Node *rhs=condition->right;
    Node *lhs=condition->left;
    //start while loop
    fprintf(file,"%s:\n",start_loop);
     /*if it is an identifier ,then we wanna mov the value in that memory location to a register
     if we have a value ,i.e an integer then we wanna mov the value into a register */
    // Generate code for lhs
    if (lhs->type == IDENTIFIER){

       Variable *var=search_variable(&code_gen_stack,lhs->value);
     if(var){
        fprintf(file, "\tmov rax, [rbp-%d]\n", var->offset);
     }else{
        perror("Not found in while loop\n");
        exit(1);
     }
    } else{
      fprintf(file, "\tmov rax, %s\n", lhs->value);
    }
   
        
     
    // Generate code for rhs
    if (rhs->type == IDENTIFIER){

      Variable *var=search_variable(&code_gen_stack,rhs->value);
      if(var){
       fprintf(file, "\tmov rbx, [rbp-%d]\n", var->offset);
    }else{
       perror("Not found in while loop\n");
       exit(1);
    }
      } else{
         fprintf(file, "\tmov rbx, %s\n", rhs->value);
      }
      //comparing the rhs and the lhs
      fprintf(file,"\tcmp rax,rbx\n");
      /*
         If the sign  is == ,it means we execute the loop if the condition if rhs and lhs are equal
         if they are not equal we wanna jump to the end of the loop,(jne)
         same for other conditions 
      */
      if(strcmp(condition->value,"!=")==0){
         fprintf(file,"\tje %s\n",end_loop);
      }else if(strcmp(condition->value,"==")==0){
         fprintf(file,"\tjne %s\n",end_loop);
      }else if(strcmp(condition->value,"<")==0){
         fprintf(file,"\tjge %s\n",end_loop);
      }else if(strcmp(condition->value,"<=")==0){
         fprintf(file,"\tjg %s\n",end_loop);
      }else if(strcmp(condition->value,">")==0){
         fprintf(file,"\tjle %s\n",end_loop);
      }else if(strcmp(condition->value,"=>")==0){
         fprintf(file,"\tjl %s\n",end_loop);
      }else {
         fprintf(stderr, "Unsupported operator in while condition: %s\n", condition->value);
         exit(1);
      }
      
      traverse(code_block,file);
      
      fprintf(file,"\tjmp %s\n",start_loop);
      fprintf(file,"%s:\n",end_loop);
      //after this we wanna traverse whatever follows the while loop,on its right.
      /*
         we had :
         { } ,to the left of { we had the code block,,and the code that follows this while loop will be atteched to } that is why we need to traverse this part,
            to avoid returning and leaving it ungenerated
      */
      traverse(root->left->right->right,file);

      return;
   }

//if statement code generation
void if_statement(Node *root,FILE *file){
   char end_label[64],else_label[64];
   //create labels
   int label_counter=if_label_counter++;
 
   sprintf(end_label, "end_if%d", label_counter);
   
    //get the code node,codition node 
    Node *condition=root->left->left;
    Node *code_block=root->left->right;
    Node *else_block=root->right;
    
    //we only wanna do this if there is an else if block
    if(else_block){
      sprintf(else_label, "_else%d", label_counter);
    }

    //get the lhs and the rhs
    Node *lhs=condition->left;
    Node *rhs=condition->right;
    // Generate code for lhs
    if (lhs->type == IDENTIFIER){
        Variable *var=search_variable(&code_gen_stack,lhs->value);
        if(var){
         fprintf(file, "\tmov rax, [rbp-%d]\n", var->offset);
        }else{
          perror("Variable not found in the code generation stack\n");
          exit(1);
        }
      }
    else
        fprintf(file, "\tmov rax, %s\n", lhs->value);

    // Generate code for rhs
    if (rhs->type == IDENTIFIER){
      Variable *var=search_variable(&code_gen_stack,rhs->value);
      if(var){
       fprintf(file, "\tmov rbx, [rbp-%d]\n", var->offset);
      }else{
        perror("Variable not found in the code generation stack\n");
        exit(1);
      }
    }
    else
        fprintf(file, "\tmov rbx, %s\n", rhs->value);

   //since we have the lhs and rhs in rax and rbx respectfully ,now we should do the conditional check (CMP)
   fprintf(file,"\tcmp rax,rbx\n");
   
   //jump based on the result
   /*logic is : 
    --If we encounter ==, it means we need to execute the 'if' block  if the comparsion is true
    so we will jump to the else or end label if they are not equal hence we use the jne(jump not equal)
    --If we encounter != ,then we wanna exeute the if block if they are not equal,hence jump if they are equal to the else or end if label
       we use je(jump equal)

    --
    

   */
   
     //If the else block is null ,jmp to the end of the if block ,but if it is not null jmp to the else block
      if(else_block){

         if (strcmp(condition->value, "==") == 0)
            fprintf(file, "\tjne %s\n", else_label); 
         else if (strcmp(condition->value, "!=") == 0)
            fprintf(file, "\tje %s\n",else_label); 
         else if (strcmp(condition->value, "<") == 0)
            fprintf(file, "\tjge %s\n", else_label); 
         else if (strcmp(condition->value, "<=") == 0)
            fprintf(file, "\tjg %s\n", else_label); 
         else if (strcmp(condition->value, ">") == 0)
            fprintf(file, "\tjle %s\n", else_label); 
         else if (strcmp(condition->value, "=>") == 0)
            fprintf(file, "\tjl %s\n", else_label);  
         else {
               fprintf(stderr, "Unsupported operator in if condition: %s\n", condition->value);
               exit(1);
           }

      }else{
         if (strcmp(condition->value, "==") == 0)
            fprintf(file, "\tjne %s\n", end_label); 
         else if (strcmp(condition->value, "!=") == 0)
            fprintf(file, "\tje %s\n",end_label);  
         else if (strcmp(condition->value, "<") == 0)
            fprintf(file, "\tjge %s\n", end_label); 
         else if (strcmp(condition->value, "<=") == 0)
            fprintf(file, "\tjg %s\n", end_label);  
         else if (strcmp(condition->value, ">") == 0)
            fprintf(file, "\tjle %s\n", end_label); 
         else if (strcmp(condition->value, ">=") == 0)
            fprintf(file, "\tjl %s\n", end_label);  
         else {
               fprintf(stderr, "Unsupported operator in if condition: %s\n", condition->value);
               exit(1);
           }
      }

   

   //traverse the code block 
  
   traverse(code_block->left,file);

   fprintf(file, "\tjmp %s\n", end_label);
   //I don't really need the else label if there is no else block
   if(else_block){
      
      fprintf(file, "%s:\n", else_label);
      traverse(else_block->left,file);
   } 
      
   
   fprintf(file, "%s:\n",end_label);
   if(else_block){
      traverse(else_block->right,file);
   }else{
      traverse(code_block->right,file);
   }
  
  
}
/*
  A do while loop should execute altleast once
  so generate the body ,then check the condition
  to the left of the do node we have {
  to the right of do we have }
  to the left of { is the code block
  and to the right of { is the while statement and the condition
  so traverse left of the curly braces only to generate the code block
  do not call traverse of the while statement ,cause that might end up recursing into the normal while loop ,
  causing error or unexpected behaviour.
*/
void do_while(Node *root,FILE *file){
   if(!root) return;
    char start_loop[64],end_loop [64];
    int label_counter=do_while_label_counter++;
    sprintf(start_loop,"do_while%d",label_counter);
    sprintf(end_loop,"end_do_while%d",label_counter);

    Node *code_block=root->left->left;
    Node *condition=root->left->right->left->left;
    //lhs and rhs of the condition
    Node *lhs=condition->left;
    Node *rhs=condition->right;
    fprintf(file,"%s:\n",start_loop);
    traverse(code_block,file);
    /*after generating the body ,we need to take care of the condition and determine whether we will continue looping or not
    mov lhs into rax
    mov rhs ito rbx
    compare them
    jump based on the comparsion result
    */
    if(lhs->type==IDENTIFIER){
      Variable *var=search_variable(&code_gen_stack,lhs->value);
      if(var){
         fprintf(file,"\tmov rax,[rbp-%d]\n",var->offset);
      }else{
         perror("Undefined variable\n");
         exit(1);
      }
    }
    if(rhs->type==INT){
       fprintf(file,"\tmov rbx,%s\n",rhs->value);
    }
    fprintf(file,"\tcmp rax,rbx\n");
    if(strcmp(condition->value,"!=")==0){
      fprintf(file,"\tje %s\n",end_loop);
    }else if(strcmp(condition->value,"==")==0){
      fprintf(file,"\tjne %s\n",end_loop);
    }else if(strcmp(condition->value,"<")==0){
      fprintf(file,"\tjge %s\n",end_loop);
    }else if(strcmp(condition->value,"<=")==0){
      fprintf(file,"\tjg %s\n",end_loop);
    }else if(strcmp(condition->value,">")==0){
     fprintf(file,"\tjle %s\n",end_loop);
    }else if(strcmp(condition->value,"=>")==0){
     fprintf(file,"\tjl %s\n",end_loop);
   }else {
    fprintf(stderr, "Unsupported operator in do while condition: %s\n", condition->value);
    exit(1);
   }
   fprintf(file,"\tjmp %s\n",start_loop);
   fprintf(file,"%s:\n",end_loop);
   //We don't wanna stop here ,traverse the rest of the subtrees
   traverse(root->right,file);
}
/*
   Just for global variables ,
   when generating code inside a function ,we search the top of the scope stack(since our scopes are just functions)
   If not found then we know it is a global variable hence we can access it through [variable name]
   
*/
void generate_data_section(Node *root,FILE *file){
   
   if(!root) return;
   if(strcmp(root->value,"int")==0 && root->left->left &&strcmp(root->left->left->value,"(")==0) return;
   /*
   if the function encounters a variable it defines it in the data section
   It might be a function though.
   */
   if( strcmp(root->value,"int")==0 && root->left->left &&strcmp(root->left->left->value,"(")!=0){
     
     
      fprintf(file,"\t%s dq 0\n",root->left->left->value);/*root->left->left because my ast is structured in a way that when I encounter int key word,its left is =,
                                                         then left of equal there is variable name ,and right is the expression or a value*/
     
   }

   generate_data_section(root->left,file);//go left
   generate_data_section(root->right,file);//recurse  right
}

/*
  This function is gonna handle: y=++x,y=x++,y=--x,y=x--
  y=++x: increment the value of x by 1 ,then assign it to y
  y=x++:assign the value of x to y,then increment the value of x by 1

  y=--x: decrement the value of x by 1 ,then assign it to y
  y=x--:assign the value of x to y,then decrement the value of x by 1
  
  The undefined variable errors I am throwing in here are not really meant for end users,
  they are meant for developer's debugging ,,
  just to be safe cause ,if the variable was undefined ,the error would have be thrown during parsing.

*/

void handle_unary_assignment(Node *root,FILE *file){
   if(!root) return;
    Variable *lhs_var=search_variable(&code_gen_stack,root->left->left->value);
    if(lhs_var){
      if(root->left->right && root->left->right->type==IDENTIFIER){
         Variable *rhs_var=search_variable(&code_gen_stack,root->left->right->value);
         if(rhs_var){
            if(root->left->right->left && strcmp(root->left->right->left->value,"--")==0){
               fprintf(file,"\tmov rax,[rbp-%d]\n",rhs_var->offset);
               fprintf(file,"\tmov [rbp-%d],rax\n",lhs_var->offset);
               fprintf(file,"\tDEC QWORD[rbp-%d]\n",rhs_var->offset);
             }else if(root->left->right->left && strcmp(root->left->right->left->value,"++")==0){
               fprintf(file,"\tmov rax,[rbp-%d]\n",rhs_var->offset);
               fprintf(file,"\tmov [rbp-%d],rax\n",lhs_var->offset);
               fprintf(file,"\tINC QWORD[rbp-%d]\n",rhs_var->offset);
             }
         }else{
            fprintf(stderr, "\033[1;31mError:\033[0m using an undefined variable, '%s' is not defined in the code generation scope stack\n",root->left->right->value);
            exit(1);
         }
         
      }else if(root->left->right && root->left->right->left && root->left->right->type==OPERATOR){
         Variable *rhs_var=search_variable(&code_gen_stack,root->left->right->left->value);
         if(rhs_var){
            if(root->left->right && strcmp(root->left->right->value,"--")==0){
               fprintf(file,"\tDEC QWORD[rbp-%d]\n",rhs_var->offset);
               fprintf(file,"\tmov rax,[rbp-%d]\n",rhs_var->offset);
               fprintf(file,"\tmov [rbp-%d],rax\n",lhs_var->offset);
               
             }else if(root->left->right && strcmp(root->left->right->value,"++")==0){
               fprintf(file,"\tINC QWORD[rbp-%d]\n",rhs_var->offset);
               fprintf(file,"\tmov rax,[rbp-%d]\n",rhs_var->offset);
               fprintf(file,"\tmov [rbp-%d],rax\n",lhs_var->offset);
             }
         }else{
            
            fprintf(stderr, "\033[1;31mError:\033[0m using an undefined variable, '%s' is not defined in the code generation scope stack\n",root->left->right->value);
            exit(1);
         }
         
      }
    }else{
       fprintf(stderr, "\033[1;31mError:\033[0m using an undefined variable, '%s' is not defined in the code generation scope stack\n",root->left->left->value);
       exit(1);
    }
    

   traverse(root->right,file);
}

/*
  
*/

void postfix_prefix_statement(Node *root,FILE *file){
   if(root->left->type==IDENTIFIER){

      Variable *var=search_variable(&code_gen_stack,root->left->value);
      if(var){
        
         if(strcmp(root->left->left->value,"++")==0){
            fprintf(file,"\tINC QWORD[rbp-%d]\n",var->offset);
         }else if(strcmp(root->left->left->value,"--")==0){
            fprintf(file,"\tDEC QWORD[rbp-%d]\n",var->offset);
         }
         
      }
   }else if(root->left->type==OPERATOR){
      Variable *var=search_variable(&code_gen_stack,root->left->left->value);
      if(var){
        
         if(strcmp(root->left->value,"++")==0){
            fprintf(file,"\tINC QWORD[rbp-%d]\n",var->offset);
         }else if(strcmp(root->left->value,"--")==0){
            fprintf(file,"\tDEC QWORD[rbp-%d]\n",var->offset);
         }
         
      }
   }

   traverse(root->right,file);
}

Node *parent=NULL;
Table *scope;
void traverse(Node *root, FILE *file) {
  
  if (!root) return;
  
  if (strcmp(root->value, "if") == 0) {
   /*
     We see an if statement ,we call the function to handle if statements
     We see a while loop ,we call a function to handle while loops
     We see a for loop, call a function to handle for loops
     We see a do while loop ,we call a function to handle do while loops
     otherwise we traverse
     for loops ,it has to be a preoder traversal to avoid traversing the code inside the loop twice
   */
     if_statement(root,file);
     }else if(strcmp(root->value,"while")==0){
      while_statement(root,file);
     }else if(strcmp(root->value,"do")==0){
         
         do_while(root,file);
     }else if(strcmp(root->value,"POSTPREFIX")==0){
       postfix_prefix_statement(root,file);
   }else if(strcmp(root->value,"UNARYASSIGNMENT")==0){
        handle_unary_assignment(root,file);
   }else if(strcmp(root->value,"int")==0 && root->left->right && strcmp(root->left->right->value,"FUNCTIONCALL")==0){
        Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->left->left->value);
        if(var){

         fprintf(file,"\tcall %s\n",root->left->right->left->value);
         fprintf(file,"\tmov QWORD[rbp-%d],rax\n",var->offset);
        }else{
           printf("Not found %s\n",root->left->left->value);
           exit(0);
        }
     
        traverse(root->right,file);
       
        
   }else{
      // Recursively process left and right subtrees first (Post-Order): left->right->root
      traverse(root->left, file);
      traverse(root->right, file);
       if(root->type==INT ){
         // fprintf(file,"\tmov rax,%s\n",root->value);
         fprintf(file,"\tpush QWORD %s\n",root->value);
          
      }else if(root->type==OPERATOR){
            //if it is an =,+=,-=,/=,*= sign then the value that is top of the stack is the computation of the left subtree.
           if(strcmp(root->value,"=")==0){
              
              Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->left->value);
              if(var){
              
                fprintf(file,"\tpop QWORD[rbp-%d]\n",var->offset);
              }else{
                fprintf(file,"\tpop QWORD [%s]\n",root->left->value);
              }
               
           }else if(strcmp(root->value,"+=")==0){

               Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->left->value);
               if(var){
               fprintf(file,"\tpop rax\n");
               fprintf(file,"\tadd QWORD[rbp-%d],rax\n",var->offset);
               }else{
               printf("Not found %s\n",root->value);
               exit(1);
               }
              
           }else if(strcmp(root->value,"-=")==0){
               Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->left->value);
               if(var){
               fprintf(file,"\tpop rax\n");
               fprintf(file,"\tsub QWORD[rbp-%d],rax\n",var->offset);
               }else{
               printf("Not found %s\n",root->value);
               exit(1);
               }
           }else if(strcmp(root->value,"*=")==0){
               Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->left->value);
               if(var){
               fprintf(file,"\tpop rax\n");
               fprintf(file,"\timul rax,QWORD[rbp-%d]\n",var->offset);
               fprintf(file,"\tmov QWORD[rbp-%d],rax\n",var->offset);
               }else{
               printf("Not found %s\n",root->value);
               exit(1);
               }
           }else if(strcmp(root->value,"*=")==0){
               Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->left->value);
               if(var){
               fprintf(file,"\tpop rax\n");
               fprintf(file,"\timul rax,QWORD[rbp-%d]\n",var->offset);
               fprintf(file,"\tmov QWORD[rbp-%d],rax\n",var->offset);
               }else{
               printf("Not found %s\n",root->value);
               exit(1);
               }
           }else if(strcmp(root->value,"/=")==0){
               Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->left->value);
               if(var){
               fprintf(file,"\tmov rax,QWORD[rbp-%d]\n",var->offset);
               fprintf(file,"\tpop rbx\n");
               fprintf(file,"\tidiv rbx\n");
               fprintf(file,"\tmov QWORD[rbp-%d],rax\n",var->offset);
               }else{
               printf("Not found %s\n",root->value);
               exit(1);
               }
        }
          
          //so basically pop the last two values pushed on the, and appy this operator on them 
          //and then finally push this result back to the stack
          // popping twice since we know if we have an operator then there must be a left value and a right value
           fprintf(file,"\tpop rdi\n");
           fprintf(file,"\tpop rax\n");
    
          if(strcmp(root->value,"+")==0) fprintf(file,"\tadd rax,rdi\n");
    
          if(strcmp(root->value,"-")==0) fprintf(file,"\tsub rax,rdi\n");
           
    
          if(strcmp(root->value,"*")==0) fprintf(file,"\timul rax,rdi\n");
    
          if(strcmp(root->value,"/")==0) fprintf(file,"\tidiv rdi\n");
         
          fprintf(file,"\tpush rax\n");
    
       }else if(root->type==IDENTIFIER && strcmp(root->value,"ASSIGN")!=0 &&strcmp(root->value,"POSTPREFIX")!=0 && !root->left ){
         
          Variable *var=hashmap_get(current_scope(&code_gen_stack)->map,root->value);
          if(var){
          fprintf(file,"\tpush QWORD [rbp-%d]\n",var->offset);
           
          }else{
           
             fprintf(file,"\tpush QWORD [%s]\n",root->value);
          }
          
         
       }else if(strcmp(root->value,"ASSIGN")==0){
            root=root->left;
       }
       if(strcmp(root->value,"return")==0 && parent && strcmp(parent->value,"main")==0){
          /*If the execution got into the main function's return statement it means we will exit the program with the return value as the exit code.
          If the return value is an integer then we move it to rdi
          If the return value is an identifier ,then we move the value at that memory location into rdi i.e mov rdi,[identifer]
          */

          fprintf(file,"\tpop rdi\n");
          fprintf(file, "\tmov rax, 60\n"); 
           /*
          We are back from traversing the current scope ,,therefore we need to clean up the stack
          return stack pinter to point to the current base pointer 
          and pop the base pointer
         */
          fprintf(file, "\tmov rsp, rbp\n"); 
          fprintf(file, "\tpop rbp\n"); 
          fprintf(file, "\tsyscall\n");
       }else if(strcmp(root->value,"return")==0){
            /*If it is return of another function then we wanna mov into rax the return value then return ,
            so now we call a function from somewhere we know the return value is in rax,,when we traverse back to return statement
            it means that we have pushed our return result onto the stack therefore we can pop it into rax
            and also we wanna restore the stack 
           */
            fprintf(file,"\tpop rax\n");
            fprintf(file, "\tmov rsp, rbp\n"); 
            fprintf(file, "\tpop rbp\n"); 
            fprintf(file, "\tret\n"); 
          
       }
        // Handle exit syscall if root contains "exit"
      if (strcmp(root->value, "exit") == 0) {
       fprintf(file, "\tpop rdi\n"); // Final result to rdi before exiting
        fprintf(file, "\tmov rax, 60\n"); // Exit syscall
       
       fprintf(file, "\tsyscall\n");
       //not checking for the semi colon since the parent is the last to be evaluated ,,post order traversal
    }
   }
}


/*
 for functions we wanna create a label for each ,corresponding to the function name...i.e 
 main function we create main label:
  main:
   main function code
   We wanna also track the function names ,so that if we reach to main's return statement ,we exit with the return value as the exit code
   i.e return 0,,exit(0)

*/


void function(Node *root,FILE *file){
  
   if(!root) return;
   //if it is not a function we return early
   
    //Not checking if root->left->left exists since we have already parsed and produced ast meaning everything is as expected
   if(strcmp(root->value,"int")==0 && strcmp(root->left->left->value,"(")==0){
      /*
       we see the main function we generate global start label since we want execution to start from here,
       any other function we will create a label from the function name
      
      */
      parent=root->left;
     
      fprintf(file,"%s:\n",root->left->value);
      scope =current_scope(&code_gen_stack);
      
      if(scope){
      fprintf(file, "\tpush rbp\n"); 
      fprintf(file, "\tmov rbp, rsp\n"); 
     
      fprintf(file, "\tsub rsp, %d\n", scope->current_offset); 
     }else{
      perror("No active scope\n");
   }
     
      traverse(root->left,file);
      pop_scope(&code_gen_stack);
   }
    
  
   function(root->left,file);
   function(root->right,file);
   
}

/*
 Generating code for global variables :
 we see a function ,we do not traverse it
*/
void generate_global_variables(Node *root,FILE *file){
   if(!root) return;
   //returning at this point because we don't want to generate code inside functions
   if(strcmp(root->value,"int")==0 && root->left->left &&strcmp(root->left->left->value,"(")==0) return;
   /*
   if the function encounters a variable it defines it in the data section
   It might be a function though.(we don't want to get here)
   */
   if( strcmp(root->value,"int")==0 && root->left->left &&strcmp(root->left->left->value,"(")!=0){
      traverse(root->left,file);
   }

   generate_global_variables(root->left,file);
   generate_global_variables(root->right,file);

}
//entry point of code generation.
void code_generator(Node *root){
   
  FILE *file=fopen("generated.asm","w");
  if(file==NULL){
    printf("File cannot be opened\n");
    exit(1);
  }

  fprintf(file,"section .data\n");
  generate_data_section(root,file);
  fprintf(file,"section .text\n");
  fprintf(file,"\tglobal _start\n");
  fprintf(file,"_start:\n");
  //generate code for global variables;;;;;;;;
  generate_global_variables(root,file);
  fprintf(file,"\tjmp main\n");
  /*
    The code generation scope stack at this point is in reverse ,,
    code generation starts from top to bottom ,so we need to reverse the stack
    to bring the scopes in a correct order of how they will be processed
  */
  reverse_codegen_stack(&code_gen_stack);
  pop_scope(&code_gen_stack);
  function(root,file);
  fclose(file);

}