#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>




#define GLOABALSPACE 5

typedef struct Node{
  char *value;
  TokenType type;
   struct Node *left;
  struct Node *right;
} Node;


Node *initialize_node(Node *node,char *val,TokenType type){
    node=malloc(sizeof(Node));
    node->value=malloc(sizeof(char)*2);
    node->value=strdup(val);
    node->type=type;
    
    
    return node;
}






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
    printf("Expected \"%s\" after \"%s\" on line %zu \n",token,prev_token.value,prev_token.line_num);
    exit(1);
}


//parse primary expression.
Node *parse_primary(Token **current_token_ptr){
    Token *token=*current_token_ptr;
    if (token->type != INT) {
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




// function to parse tokens and create AST
 
  
Node *parser(Token *tokens) {
    Token *current_token=&tokens[0];

    Node *current=malloc(sizeof(Node));
    Node *root=malloc(sizeof(Node));
    Node *left=malloc(sizeof(Node));
   
    root=initialize_node(root,"PROGRAM",BEGINNING);
    
    current=root;
    
  

    while(strcmp(current_token->value,"EOF")){

      switch(current_token->type){
         case KEYWORD:
           if (strcmp(current_token->value,"exit")==0){
               Node *exit_node=malloc(sizeof(Node));
               exit_node=initialize_node(exit_node,current_token->value,current_token->type);
               current->right=exit_node;
               current=exit_node;
               current_token++;

               //next token should  be open parenthesis else we terminate the execution with an error
               
               if (strcmp(current_token->value,"EOF")==0){
                 end_of_tokens_error(current_token->line_num);
               }

               if (current_token->type==SEPARATOR && strcmp(current_token->value,"(")==0){
                   
                   Node *open_parens_node=initialize_node(NULL,current_token->value,current_token->type);
                   exit_node->left=open_parens_node;
                   current=open_parens_node;
                   current_token++;
               }else{
                 missing_token_error("(",*(current_token-1));
                 
               }

                //next token should be an integer literal else we terminate the execution with an error

               if (strcmp(current_token->value,"EOF")==0){
                  end_of_tokens_error(current_token->line_num);
               }

               //next token should be an integer literal or an identifier,it can also be an expression.

               if (current_token->type==INT){
                
                Node *expr = parse_expression(&current_token);
                current->left = expr;
               }
              if(current_token->type==SEPARATOR && strcmp(current_token->value,")")==0){
                
                Node *close_parens_node=initialize_node(NULL,current_token->value,current_token->type);
                current->right=close_parens_node;
                current_token++;

              }else{
                 missing_token_error(")",*(current_token-1));
              }

              //next token should be a semi colon else we terminate the execution with an error

              if (strcmp(current_token->value,"EOF")==0){
                end_of_tokens_error(current_token->line_num);
              }

              if (current_token->type==SEPARATOR && strcmp(current_token->value,";")==0){
                
                Node *semi_colon_node=initialize_node(NULL,current_token->value,current_token->type);
                exit_node->right=semi_colon_node;
                current_token++;

              }else{
                missing_token_error(";",*(current_token-1));
              }

           }

           
         
         default:
           break;
      }
   
   }


  
   print_tree(root,5);

   return root;
    
}
