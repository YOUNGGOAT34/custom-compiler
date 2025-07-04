#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "./lexer/lexer.h"
#include "parser/parser.h"
#include "code_generator/code_generator.h"
#include "symbol_table/hashmap.h"
#include "scope/stack.h"



int main(void){
    FILE *file=fopen("test.c","r");
     if (file == NULL) {
        
        perror("Failed to open file");
        return 1;
    }
  
   int arr[10];
   memset(&arr,0,10*sizeof(int));
  Token *tokens=lexer(file);

  //  print_tokens(tokens);
  
   Node *root= parser(tokens);
  
   code_generator(root);
  
   system("nasm -f elf64 generated.asm -o generated.o");
   system("ld generated.o -o generated");
 
  // // print_table();
  
  return WEXITSTATUS(system("./generated"));
  // return 0;

}






