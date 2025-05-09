#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "./lexer/lexer.h"
#include "parser/parser.h"
#include "code generator/code_generator.h"
#include "symbol table/hashmap.h"




int main(){
    FILE *file=fopen("../test.g","r");
     if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    
  Token *tokens=lexer(file);
  //  print_tokens(tokens);
   Node *root= parser(tokens);
   
   code_generator(root);

   system("nasm -f elf64 ../generated.asm -o generated.o");
   system("ld generated.o -o generated");
   printf("\n");
  
  print_table();

  
  return WEXITSTATUS(system("./generated"));

  

 

  

}




