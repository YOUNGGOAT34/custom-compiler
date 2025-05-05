#include<stdio.h>
#include<stdlib.h>


void generate_data_section(Node *root,FILE *file){
 
   if(!root) return;
   //if the function encounters a variable it defines it in the data section
   if( strcmp(root->value,"int")==0 &&root->left){
     
      fprintf(file,"\t%s dq 0\n",root->left->left->value);//root->left->left because my ast is structured in a way that when I encounter int key word,its left is =,
                                                          //then left of equal there is variable name ,and right is the expression or a value
     
   }

   generate_data_section(root->left,file);//go left
   generate_data_section(root->right,file);//recurse  right
}

void traverse(Node *root, FILE *file) {
  if (!root) return;

  // Recursively process left and right subtrees first (Post-Order): left->right->root
  traverse(root->left, file);
  traverse(root->right, file);

   if(root->type==INT){
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

   }else if(root->type==IDENTIFIER){
      fprintf(file,"\tmov rax,[%s]\n",root->value);
      fprintf(file,"\tpush rax\n");
   }

  // Handle exit syscall if root contains "exit"
  if (strcmp(root->value, "exit") == 0) {
      fprintf(file, "\tpop rdi\n"); // Final result to rdi before exiting
      fprintf(file, "\tmov rax, 60\n"); // Exit syscall
      fprintf(file, "\tsyscall\n");
      //not checking for the semi colon since the parent is the last to be evaluated ,,post order traversal
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


