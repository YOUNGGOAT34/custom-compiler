#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <assert.h>
#include <string.h>
#include "../symbol table/hashmap.c"
#include "../scope/stack.c"



#define GLOABALSPACE 5

 //for the symbol table


typedef struct Node{
  char *value;
  TokenType type;
   struct Node *left;
  struct Node *right;
} Node;


//Initialize node

Node *initialize_node(Node *node,char *val,TokenType type){
    node=malloc(sizeof(Node));
    node->value=malloc(sizeof(char)*2);
    node->value=strdup(val);
    node->type=type;
    
    
    return node;
}




//visualize the AST

void print_tree(Node *root,int space){
    if(root==NULL) return;
    space=space+GLOABALSPACE;

   
    print_tree(root->right,space);
    printf("\n");

    for(int i=GLOABALSPACE;i<space;i++){
        printf(" ");

    }
    printf("%s\n",root->value);
    
    print_tree(root->left,space);

}


void end_of_tokens_error(size_t line){
  printf("Unexpected end of tokens on line %zu\n",line);
  exit(1);
}

void missing_token_error(char *token,Token prev_token){
    // printf("Expected \"%s\" after \"%s\" on line %zu \n",token,prev_token.value,prev_token.line_num);
    fprintf(stderr, "\033[1;31mError:\033[0m ( %zu): Expected '%s' after '%s'  \n",prev_token.line_num, prev_token.value,token);
    exit(1);
}




//parse primary expression.
Node *parse_primary(Token **current_token_ptr){
    Token *token=*current_token_ptr;
     if(token->type==IDENTIFIER){
        //get the current scope, then get the variable
        
        Variable *variable=search_variable(token->value);
       
        if(variable==NULL){
          fprintf(stderr, "\033[1;31mError:\033[0m ( %zu): '%s' is not \n", token->line_num,variable->name);
          exit(1);
        };
     }
    if (token->type != INT && token->type!=IDENTIFIER) {
      
      missing_token_error("integer literal", *token);
    }
    
    Node *node=initialize_node(NULL,token->value,token->type);
     token++;
    *current_token_ptr=token;
    return node;
}

//parse multiplication and division.
Node *parse_multiplication(Token **current_token_ptr) {
  Node *left = parse_primary(current_token_ptr); 

  Token *token = *current_token_ptr;
  while (token->type == OPERATOR && (strcmp(token->value, "*") == 0 || strcmp(token->value, "/") == 0)) {
      Node *op_node = initialize_node(NULL, token->value, token->type);
      token++;
      Node *right = parse_primary(&token);
      op_node->left = left;
      op_node->right = right;
      left = op_node;

      
  }

  *current_token_ptr=token;

  return left;
}


// A method to pass expressions i.e 1+1+1.........

Node *parse_expression(Token **current_token_ptr) {
  Node *left = parse_multiplication(current_token_ptr);  // First handle multiplication/division
  
  Token *token = *current_token_ptr;
  while (token->type == OPERATOR && (strcmp(token->value, "+") == 0 || strcmp(token->value, "-") == 0)) {
      Node *op_node = initialize_node(NULL, token->value, token->type);
      token++;

      Node *right = parse_multiplication(&token);  // Multiplication gets handled first inside addition

      op_node->left = left;
      op_node->right = right;
      left = op_node;

      
  }

  *current_token_ptr=token;

  return left;
}


//function to check if a variableis defined before using it
bool check_variable(char *name){
   Variable *var=search_variable(name);;
   if(var==NULL) return false;

   return true;
}

//handle exit system call
void handle_exit_system(Node *current,Token **current_token){
      Token *token=*current_token;
     
      Node *exit_node=initialize_node(NULL,token->value,token->type);
      current->right=exit_node;
      current=exit_node;
      token++;

      //next token should  be open parenthesis else we terminate the execution with an error
      
      if (strcmp(token->value,"EOF")==0){
        end_of_tokens_error(token->line_num);
      }

      if (token->type==SEPARATOR && strcmp(token->value,"(")==0){
          Node *open_parens_node=initialize_node(NULL,token->value,token->type);
          exit_node->left=open_parens_node;
          current=open_parens_node;
          token++;
      }else{
        missing_token_error("(",*(token-1));
        
      }

      //next token should be an integer literal else we terminate the execution with an error

      if (strcmp(token->value,"EOF")==0){
        end_of_tokens_error(token->line_num);
      }
      //next token should be an integer literal or an identifier,it can also be an expression.
      if (token->type==INT || token->type==IDENTIFIER){
      
      Node *expr = parse_expression(&token);
      current->left = expr;
      }
    if(token->type==SEPARATOR && strcmp(token->value,")")==0){
      
      Node *close_parens_node=initialize_node(NULL,token->value,token->type);
      current->right=close_parens_node;
      token++;
    }else{
        missing_token_error(")",*(token-1));
    }

    //next token should be a semi colon else we terminate the execution with an error
    if (strcmp(token->value,"EOF")==0){
      end_of_tokens_error(token->line_num);
    }
    if (token->type==SEPARATOR && strcmp(token->value,";")==0){
      
      Node *semi_colon_node=initialize_node(NULL,token->value,token->type);
      exit_node->right=semi_colon_node;
      token++;
    }else{
      missing_token_error(";",*(token-1));
    }
    *current_token=token;
}

//variable redefination
void variable_redefination(Variable *var){
  fprintf(stderr, "\033[1;31mError:\033[0m Variable '%s' is already defined (line %zu)\n", var->name, var->line_number);

   exit(1);
   
}


//create variables

Node *create_variables(Node *current,Token **current_token_ptr){
    
    Token *token=*current_token_ptr;
    //allocate memory for the variable
    Variable *variable=malloc(sizeof(Variable));
    Node *var_node=initialize_node(NULL,token->value,token->type);
    //variable type i.e int,char ,float etc....
    variable->type=token->value;
    
    current->left=var_node;
    token++;
    // The next expected is an identifie;
    if(strcmp(token->value,"EOF")==0){
       end_of_tokens_error(token->line_num);
    }
    if (token->type==IDENTIFIER){
       token++;

       if(strcmp(token->value,"EOF")==0){
        end_of_tokens_error(token->line_num);
      }

     Node *operator_node=initialize_node(NULL,token->value,token->type);

      if (token->type==OPERATOR){
        var_node->left=operator_node;
        token--;
       }else{
        missing_token_error("operator",*(token-1));
       }

       Node *identifier_node=initialize_node(NULL,token->value,token->type);
       //check if the variable has been already defined
        Variable *existing = search_variable(token->value);
        if (existing != NULL) {
            variable_redefination(existing); // handle the redeclaration error
        }
       //name of the variable(identifier)
       variable->name = strdup(token->value);
       variable->type = strdup(token->value);
       //insert the variable into the symbol table.
       hashmap_insert(current_scope()->map, variable->name, variable);
       
       operator_node->left=identifier_node;
       token++;
       token++;
       //next token expected should be an integer or an expression
      if(strcmp(token->value,"EOF")==0){
      end_of_tokens_error(token->line_num);
       }
    
      if (token->type==INT){
        
      // Node *integer_node=initialize_node(NULL,token->value,token->type);
      operator_node->right=parse_expression(&token);
      // token++;
    }else{
      missing_token_error("integer",*(token-1));
    }
    }else{
       missing_token_error("identifier",*(token-1));
    }
   

  //next token expected should be a semi colon

  if(strcmp(token->value,"EOF")==0){
    end_of_tokens_error(token->line_num);
    }
  
  if (token->type==SEPARATOR){
    Node *semi_colon_node=initialize_node(NULL,token->value,token->type);
    var_node->right=semi_colon_node;
    
    token++;
  }else{
    missing_token_error(";",*(token-1));
  }
  
  *current_token_ptr=token;
  return var_node->right;
  
}

//function to handle variable reassignment
Node *handle_variable_reassignment(Node *node,Token **current_token_ptr){
      Token *token=*current_token_ptr;
      Node *update_variable_node=initialize_node(NULL,"UPDATE",token->type);
      node->left=update_variable_node;
      token++;
    //At this point the current token should be an assignment operator
    if(token->type==OPERATOR &&strcmp(token->value,"=")==0){
      // Node *curr=handle_identifier(identifier_node,&current_token);
      // current=curr;
      Node *op_node=initialize_node(NULL,token->value,token->type);
      update_variable_node->left=op_node;
      token--;
      //we go back to the identifier
      Node *identifier_node=initialize_node(NULL,token->value,token->type);
      op_node->left=identifier_node;
      //skip twice since we have already processed the assignment operator
      token++;
      token++;
      if(strcmp(token->value,"EOF")==0){
      end_of_tokens_error(token->line_num);
      }
      // at this point the current token should be an identifier ,integer or an expression
      if(token->type==IDENTIFIER || token->type==INT){
          op_node->right=parse_expression(&token);
      }else{
        missing_token_error("identifier, integer or an expression",*(token-1));
      }
      //At this point we should have a semi colon to terminate this our statement.
      if(token->type==SEPARATOR&&strcmp(token->value,";")==0){
        Node *semi_colon_node=initialize_node(NULL,token->value,token->type);
        update_variable_node->right=semi_colon_node;

        token++;
      }else{
        missing_token_error(";",*(token-1));
      }

    }else{
    missing_token_error("=",*(token-1));
    }

    *current_token_ptr=token;

   return update_variable_node->right;
   
}

// function to parse tokens and create AST
Node *parser(Token *tokens) {
   
    Token *current_token=&tokens[0];

    Node *current=malloc(sizeof(Node));
    Node *root=malloc(sizeof(Node));
    Node *left=malloc(sizeof(Node));
   
    root=initialize_node(root,"START",BEGINNING);
    //initialize a symbol table for the global scope
    Table *global_table=create_table();
     //Initial I need to have this scope pushed onto the stack
      // Push the global scope initially
      push_scope(global_table);
      current=root;
    
    while(strcmp(current_token->value,"EOF")){

      switch(current_token->type){
         case KEYWORD:
           if (strcmp(current_token->value,"exit")==0){
               handle_exit_system(root,&current_token);

           }else if(strcmp(current_token->value,"int")==0){
               Node *var_left=create_variables(current,&current_token);
               current=var_left;
                
               
           }
          break;
        
        case IDENTIFIER:
           //variable updating or reassignment
           // check if the variable is declared before updating it
           if(current_token->type==IDENTIFIER){
                
               if(!check_variable(current_token->value)){
                  fprintf(stderr, "\033[1;31mError:\033[0m Variable '%s' is not defined (line %zu)\n", current_token->value, current_token->line_num);
                  exit(1);
           }
             
            Node *semi_colon_node=handle_variable_reassignment(current,&current_token);
            current=semi_colon_node;
          }
          
           break; 

        case SEPARATOR:
          if (strcmp(current_token->value,"{")==0){
             //this indicates that we are entering a new scope
            
              Table *new_scope=create_table();
             
              push_scope(new_scope);
              
          }else if(strcmp(current_token->value,"}")==0){
             //this indicates that we are exiting a scope
              pop_scope();
              
              
          }
          current_token++;
          
          break;
        case OPERATOR:
          
          current_token++;
          break;

        case INT:
           
           current_token++;
           break;
          
         default:
           break;
      }
   
   }


  
   print_tree(root,5);

   return root;
    
}
