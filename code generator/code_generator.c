#include<stdio.h>
#include<stdlib.h>
#include "code_generator.h"

static int if_label_counter=0;
static int while_label_counter=0,while_counter=0;
void traverse(Node *root, FILE *file);

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
    if (lhs->type == IDENTIFIER)
        fprintf(file, "\tmov rax, [%s]\n", lhs->value);
    else
        fprintf(file, "\tmov rax, %s\n", lhs->value);

    // Generate code for rhs
    if (rhs->type == IDENTIFIER)
        fprintf(file, "\tmov rbx, [%s]\n", rhs->value);
    else
        fprintf(file, "\tmov rbx, %s\n", rhs->value);
    //comparing the rhs and the lhs
    fprintf(file,"\tcmp rax,rbx\n");
    if(strcmp(condition->value,"!=")==0){
        fprintf(file,"\tje %s\n",end_loop);
    }

    traverse(code_block,file);
   
    fprintf(file,"\tjmp %s\n",start_loop);
    fprintf(file,"%s:\n",end_loop);
    traverse(root->left->right->right,file);

    return;
}


//if statement code generation
void if_statement(Node *root,FILE *file){
   char end_label[64],else_label[64];
   //create labels
   int label_counter=if_label_counter++;
 
   sprintf(end_label, ".end_if%d", label_counter);
   
    //get the code node,codition node 
    Node *condition=root->left->left;
    Node *code_block=root->left->right;
    Node *else_block=root->right;
    //we only wanna do this if there is an else if block
    if(else_block){
      sprintf(else_label, "._else%d", label_counter);
    }
    //get the lhs and the rhs
    Node *lhs=condition->left;
    Node *rhs=condition->right;
    // Generate code for lhs
    if (lhs->type == IDENTIFIER)
        fprintf(file, "\tmov rax, [%s]\n", lhs->value);
    else
        fprintf(file, "\tmov rax, %s\n", lhs->value);

    // Generate code for rhs
    if (rhs->type == IDENTIFIER)
        fprintf(file, "\tmov rbx, [%s]\n", rhs->value);
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
         else if (strcmp(condition->value, ">=") == 0)
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
   if(else_block) fprintf(file, "%s:\n", else_label);
   traverse(else_block,file);
   fprintf(file, "%s:\n",end_label);

  
}

void generate_data_section(Node *root,FILE *file){
   
   if(!root) return;
   if(strcmp(root->value,"while")==0) while_counter++;
   //if the function encounters a variable it defines it in the data section
   if( strcmp(root->value,"int")==0 &&root->left){
     
      fprintf(file,"\t%s dq 0\n",root->left->left->value);/*root->left->left because my ast is structured in a way that when I encounter int key word,its left is =,
                                                         then left of equal there is variable name ,and right is the expression or a value*/
     
   }

   generate_data_section(root->left,file);//go left
   generate_data_section(root->right,file);//recurse  right
}

void traverse(Node *root, FILE *file) {
  if (!root) return;
   
  if (strcmp(root->value, "if") == 0) {
   if_statement(root,file);
   // return;
   
   }else if(strcmp(root->value,"while")==0){
      
      while_statement(root,file);
    
      
   }else{


      // Recursively process left and right subtrees first (Post-Order): left->right->root
      traverse(root->left, file);
      traverse(root->right, file);
       
      
       if(root->type==INT ){
         fprintf(file,"\tmov rax,%s\n",root->value);
         fprintf(file,"\tpush rax\n");
         }else if(root->type==OPERATOR){
           if(strcmp(root->value,"=")==0){
              //if it is an equal sign then the value that is top of the stack is the computation of the left subtree.
              fprintf(file,"\tpop rbx\n");
              fprintf(file,"\tmov [%s],rbx\n",root->left->value);
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
    
       }else if(root->type==IDENTIFIER && strcmp(root->value,"UPDATE")){
          
          fprintf(file,"\tmov rax,[%s]\n",root->value);
          fprintf(file,"\tpush rax\n");
       }else if(strcmp(root->value,"UPDATE")==0){
          root=root->left;
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




void code_generator(Node *root){
  
 
  FILE *file=fopen("../generated.asm","w");
  if(file==NULL){
    printf("File cannot be opened\n");
    exit(1);
  }

  
  fprintf(file,"section .data\n");
  
  generate_data_section(root,file);

  fprintf(file,"section .text\n");
  fprintf(file,"\tglobal _start\n");
  fprintf(file,"_start:\n");
 
  traverse(root,file);
  fclose(file);

}