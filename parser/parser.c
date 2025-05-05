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

    if (token->type != INT && token->type!=IDENTIFIER) {
      printf("%u\n",token->type);
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


//handle exit system call
void handle_exit_system(Node *current,Token **current_token){
      Token *token=*current_token;
      Node *exit_node=malloc(sizeof(Node));
      exit_node=initialize_node(exit_node,token->value,token->type);
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


//create variables

Node *create_variables(Node *current,Token **current_token_ptr){
    Token *token=*current_token_ptr;
    
    Node *var_node=initialize_node(NULL,token->value,token->type);
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
    //The next token expected is assignment operator

    

   

   

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

// function to parse tokens and create AST
 
  
Node *parser(Token *tokens) {
    Token *current_token=&tokens[0];

    Node *current=malloc(sizeof(Node));
    Node *root=malloc(sizeof(Node));
    Node *left=malloc(sizeof(Node));
   
    root=initialize_node(root,"START",BEGINNING);
    
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
           
           current_token++;
           break; 

        case SEPARATOR:
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
