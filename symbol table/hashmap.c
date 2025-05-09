#include <stdio.h>
#include <string.h>
#include <stdbool.h>
// #include "hashmap.h"
#define TABLE_SIZE 10

typedef struct variable{
    char *name;
    char *type;
    size_t line_number;
    struct variable *next;
}Variable;
Variable *table[TABLE_SIZE];

unsigned int hash(char *name){
    int length=strlen(name);
    unsigned int hash_value=0;
    for (int i=0;i<length;i++){
        
        hash_value+=name[i];
        hash_value*=name[i];
        
    }
    return hash_value%TABLE_SIZE;
}


// function to create the table.

void create_table(){
    for (int i=0;i<TABLE_SIZE;i++){
         table[i]=NULL;
        
        
    }
}

//print table

void print_table(){
    
    printf("\tindex\tname\t\tage\n");
    
    for(int i=0;i<TABLE_SIZE;i++){
        Variable *variable=table[i];
       
        printf("\n");
        if(variable!=NULL){
            
            printf("\t%i\t",i);
            
            while(variable){
               printf("%s\t%s\t%zu-->  ",variable->name,variable->type,variable->line_number);
               variable=variable->next;
            }
            printf("NULL\n");
            printf("\n");
      
        }else{
            printf("\n");
            printf("\t%i\t----------------------------------------------------------------\n",i);
            printf("\n");
        }
         
        
    }
}
// to avoid collison we use: chaining,open addressing
//In chaining we find the index ,if there is an entry there we chain this new entry to the existing entry (something like a linked list)
//int open addressing we find the index ,if there is an existing entry ,we go through the table to find an empty spot to insert there
//I'm gonna go with chaining in this case

bool insert(Variable *variable){
  if(variable==NULL) return false;
  int index=hash(variable->name);
  if(table[index]!=NULL){
    variable->next=table[index];
    table[index]=variable;
  }
  table[index]=variable;
  return true;
}

//delete variable
void delete(char *name){
    int index=hash(name);
    Variable *variable=table[index];

    Variable *prev=NULL;

    while(variable!=NULL&& strcmp(variable->name,name)!=0){
        prev=variable;
        variable=variable->next;
    }

    if(variable !=NULL){
        if (prev!=NULL){
            prev->next=variable->next;
        }else{typedef struct variable{
            char *name;
            char *type;
          
            struct variable *next;
        }Variable;
            table[index]=NULL;//the item to delete was infront of the list.
        }
        
    }else{
        printf("Not found\n");
    }

    return ;
}



//search variable
Variable *search_variable(char *name){
    int index=hash(name);
    Variable *variable=table[index];
    
    while(variable!=NULL && strcmp(variable->name,name)!=0){
        variable=variable->next;
    }

    if(variable==NULL){
        return NULL;
    }
    

    return variable;
}

