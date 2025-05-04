#include<stdio.h>
#include<stdlib.h>

void traverse(Node *root, FILE *file) {
  if (!root) return;

  // Recursively process left and right subtrees first (Post-Order): left->right->root
  traverse(root->left, file);
  traverse(root->right, file);

   if(root->type==INT){
     fprintf(file,"\tmov rax,%s\n",root->value);
     fprintf(file,"\tpush rax\n");

   }else if(root->type==OPERATOR){
      //so basically pop the last two values pushed on the, and appy this operator on them 
      //and then finally push this result back to the stack
      fprintf(file,"\tpop rdi\n");
      fprintf(file,"\tpop rax\n");

      if(strcmp(root->value,"+")==0) fprintf(file,"\tadd rax,rdi\n");

      if(strcmp(root->value,"-")==0) fprintf(file,"\tsub rax,rdi\n");
       

     if(strcmp(root->value,"*")==0) fprintf(file,"\timul rax,rdi\n");

     if(strcmp(root->value,"/")==0) fprintf(file,"idiv rdi\n");
     
   

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

  fprintf(file,"section .text\n");
  fprintf(file,"\tglobal _start\n");
  fprintf(file,"_start:\n");
 
  traverse(root,file);
  fclose(file);

}


