
#include "parser.h"


#define GLOABALSPACE 5



//The table will be global to track functions
FunctionTable *function_table;

//A helper function to calculte the size of data types i.e int=4,char=1,double=8,float=4,,and I am gonna just have default as 4
size_t size_of_type(const char *type) {
  if (strcmp(type, "int") == 0) return 8;
  else if (strcmp(type, "char") == 0) return 1;
  else if (strcmp(type, "float") == 0) return 4;
  else if (strcmp(type, "double") == 0) return 8;
 
  return 4; 
}
//Initialize node

Node *initialize_node(char *val,TokenType type){
    Node *node=malloc(sizeof(Node));
    node->value=malloc(sizeof(char)*2);
    node->value=strdup(val);
    node->type=type;
    node->left = NULL;
    node->right = NULL;

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
    fprintf(stderr, "\033[1;31mError:\033[0m ( %zu): Expected '%s' after '%s'  \n",prev_token.line_num,token,prev_token.value);
    exit(1);
}


//Function to create functions
/*
  so I go throught the input ,encounter a data type i.e int, char or something else ,I know it might be a variable or a function
  The program will know that it is a function if after the identifier ,it sees a ( ..

  Since it is certain that we have upto the ( ,there is no point of checking that there might be unexpected token or end of tokens
  We already checked it in the create variable function
*/

Node *create_function(Node *node,Token **current_token_ptr){
  //create a new Function variable..
 
  /*
  What we need :
  function name ,
  return type,
  line number,
  parameter: name and return type
  paramer count

  
  */
      Token *token=*current_token_ptr;
      Node *return_type_node=initialize_node(token->value,token->type);
      char *return_type=token->value;
      Param *params=NULL;
      node->left=return_type_node;
      token++;
      //Identifier node
      Node *identifier_node=initialize_node(token->value,token->type);
      char *name=token->value;
      size_t line_number=token->line_num;
      if (get_function(function_table, name) != NULL) {
        fprintf(stderr, "\033[1;31mError:\033[0m Redefinition of function '%s' at line %zu ,'%s' was initially defined on line %zu\n", name, token->line_num,name,get_function(function_table,name)->line_number);
        exit(1);  // or handle gracefully
    }
      return_type_node->left=identifier_node;
      token++;
      Node *open_parens_node=initialize_node(token->value,token->type);
      identifier_node->left=open_parens_node;
      token++;
      //Parameters
      int params_capacity=4;
      int param_count=0;
      params=malloc(sizeof(Param)*params_capacity);
   while(strcmp(token->value,")")!=0){
     if(param_count>=params_capacity){
       
       params_capacity *=2;
       params=realloc(params,sizeof(Param)*param_count);
     }
      //parameter type
     if(strcmp(token->value,"int")==0){
        Param p;
        p.type=token->value;
        token++;
        //paramer identifeir
        if(token->type==IDENTIFIER){
           p.name=token->value;
           token++;
            //This is a coma
            /*
             A parameter is fully defined and it can be added to parameter list of this function
            */
           params[param_count++]=p;
          
           if(strcmp(token->value,",")==0){
            token++;
           }else if(strcmp(token->value,")")!=0){
            fprintf(stderr, "\033[1;31mError:\033[0m Expected a , on line %zu\n",token->line_num);
            exit(1);
           }
        }else{
          fprintf(stderr, "\033[1;31mError:\033[0m Expected parameter identifier after return type on line %zu\n",token->line_num);
          exit(1);
        }
        
     }else{
      fprintf(stderr, "\033[1;31mError:\033[0m Expected parameter return type on line %zu\n",token->line_num);
      exit(1);
     }
    
     
     
   }
 //close parenthesis node
 if(strcmp(token->value,")")==0){
    
    Node *close_parens_node=initialize_node(token->value,token->type);
    open_parens_node->right=close_parens_node;
     token++;
     if(strcmp(token->value,"EOF")==0){
       end_of_tokens_error(token->line_num);
     }
     //This is supposed to be open curly braces
     if(strcmp(token->value,"{")==0){
        Table *scope_table=create_table();
        Table *codegen_table=create_table();
        push_scope(&scope_stack,scope_table);
        push_scope(&code_gen_stack,codegen_table);
        Node *open_curly_node=initialize_node(token->value,token->type);
        open_parens_node->left=open_curly_node;
        node=open_curly_node;
        token++;
        while(strcmp(token->value,"return")!=0){
          if(strcmp(token->value,"int")==0){
             node=create_variables(node,&token);
          }else if(token->type==IDENTIFIER){
            
            node=handle_variable_reassignment(node,&token);
          }else if(strcmp(token->value,"while")==0){
            node=while_statement_generation(node,&token);
          }else if(strcmp(token->value,"if")==0){
            node=if_statement_generation(node,&token);
          }
        }

        //return statement
        if(strcmp(token->value,"return")==0){
          Node *return_statement=initialize_node(token->value,token->type);
          node->left=return_statement;
          token++;
          /*
           It is not necessarily that the return value is always a number or an identifier
           It might be an expression therefore we need to parse this as a normal expression.
          */
          if(token->type==IDENTIFIER || token->type==INT){
            
            Node *return_value_node=parse_expression(&token);
            
            return_statement->left=return_value_node;
            
            //semi colon
            if(strcmp(token->value,";")==0){
              Node *semi_colon_node=initialize_node(token->value,token->type);
              return_statement->right=semi_colon_node;
              token++;
            }else{
              missing_token_error(";",*(token-1));
            }
           
          }else{
             missing_token_error("integer or an identifier",*(token-1));
          }
         

        }else{
          missing_token_error("return",*(token-1));
        }
        
       
        //close curly
        if(strcmp(token->value,"}")==0){
         Node *close_curly_node=initialize_node(token->value,token->type);
         return_type_node->right=close_curly_node;
         node=close_curly_node;
        }else{
          missing_token_error("}",*(token-1));
        }
     }else{
        missing_token_error("{",*(token-1));
     }
    

     insert_function(function_table,name,return_type,params,param_count,line_number);
    //  pop_scope(&scope_stack);
 }


 
 *current_token_ptr=token;
 pop_scope(&scope_stack);
 return node;
}


//parse primary expression.
Node *parse_primary(Token **current_token_ptr){
    Token *token=*current_token_ptr;
    
     if(token->type==IDENTIFIER){
        //get the current scope, then get the variable
        
        Variable *variable=search_variable(&scope_stack,token->value);
       
        if(variable==NULL){
        
          fprintf(stderr, "\033[1;31mError:\033[0m ( %zu): '%s' is not defined \n", token->line_num,token->value);
          exit(1);
        };
     }
    
    if (token->type != INT && token->type!=IDENTIFIER) {
       
      missing_token_error("integer literal", *token);
    }
    
   
    Node *node=initialize_node( token->value,token->type);
   
     token++;
    *current_token_ptr=token;
    return node;
}

//parse multiplication and division.
Node *parse_multiplication(Token **current_token_ptr) {
 
  Node *left = parse_primary(current_token_ptr); 
  
  Token *token = *current_token_ptr;
 
  while (token->type == OPERATOR && (strcmp(token->value, "*") == 0 || strcmp(token->value, "/") == 0)) {
    
      Node *op_node = initialize_node( token->value, token->type);
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
      Node *op_node = initialize_node( token->value, token->type);
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
   Variable *var=search_variable(&scope_stack,name);;
   if(var==NULL) return false;

   return true;
}

//handle exit system call
void handle_exit_system(Node *current,Token **current_token){
      
      Token *token=*current_token;
       
      Node *exit_node=initialize_node(token->value,token->type);
      current->right=exit_node;
      current=exit_node;
      token++;
     

      //next token should  be open parenthesis else we terminate the execution with an error
      
      if (strcmp(token->value,"EOF")==0){
        end_of_tokens_error(token->line_num);
      }
      

      if (token->type==SEPARATOR && strcmp(token->value,"(")==0){
          Node *open_parens_node=initialize_node(token->value,token->type);
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
      
      Node *close_parens_node=initialize_node(token->value,token->type);
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
      
      Node *semi_colon_node=initialize_node(token->value,token->type);
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
   
    
    Node *var_node=initialize_node(token->value,token->type);
    //variable type i.e int,char ,float etc....
    char *type=token->value;
   
    current->left=var_node;
    token++;
    // The next expected is an identifier;
    if(strcmp(token->value,"EOF")==0){
       end_of_tokens_error(token->line_num);
    }
   
    if (token->type==IDENTIFIER){
       token++;
      
       if(strcmp(token->value,"EOF")==0){
        end_of_tokens_error(token->line_num);
      }
     
      //check if it is a function
      if(strcmp(token->value,"(")==0) {
       
        return create_function(current,current_token_ptr);
      }

    
     Node *operator_node=initialize_node(token->value,token->type);

      if (token->type==OPERATOR){
        var_node->left=operator_node;
        token--;
       }else{
        missing_token_error("operator",*(token-1));
       }

       Node *identifier_node=initialize_node(token->value,token->type);
       char *identifier_name=token->value;
       
       //insert the variable into the symbol table.
       if (hashmap_get(current_scope(&scope_stack)->map, identifier_name) != NULL ||
         hashmap_get(current_scope(&code_gen_stack)->map, identifier_name) != NULL) {
      
        fprintf(stderr, "Error: variable '%s' already declared\n", identifier_name);
          exit(1);  \
      }
       
       table_insert_variable(current_scope(&scope_stack),identifier_name, type, token->line_num, size_of_type(type));
       table_insert_variable(current_scope(&code_gen_stack), identifier_name, type, token->line_num, size_of_type(type));
      
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
    Node *semi_colon_node=initialize_node(token->value,token->type);
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
      // if(!check_variable(token->value)) {
      //   perror("not defined\n");
      //   exit(1);
      // }
      Node *update_variable_node=initialize_node("UPDATE",token->type);
      node->left=update_variable_node;
      token++;
    //At this point the current token should be an assignment operator
    if(token->type==OPERATOR &&strcmp(token->value,"=")==0){
      // Node *curr=handle_identifier(identifier_node,&current_token);
      // current=curr;
      Node *op_node=initialize_node(token->value,token->type);
      update_variable_node->left=op_node;
      token--;
      //we go back to the identifier
      if(token->type==IDENTIFIER && !check_variable(token->value)){
          
        fprintf(stderr, "\033[1;31mError:\033[0m Variable '%s' is not defined (line %zu)\n", token->value, token->line_num);
        exit(1);
      }
      Node *identifier_node=initialize_node(token->value,token->type);
      op_node->left=identifier_node;
      //skip twice since we have already processed the assignment operator
      token++;
      token++;
      if(strcmp(token->value,"EOF")==0){
      end_of_tokens_error(token->line_num);
      }
      // at this point the current token should be an identifier ,integer or an expression
      if(token->type==IDENTIFIER || token->type==INT){
      
        if(token->type==IDENTIFIER && !check_variable(token->value)){
          
          fprintf(stderr, "\033[1;31mError:\033[0m Variable '%s' is not defined (line %zu)\n", token->value, token->line_num);
          exit(1);
        }
          op_node->right=parse_expression(&token);
      }else{
        missing_token_error("identifier, integer or an expression",*(token-1));
      }
      //At this point we should have a semi colon to terminate this our statement.
      if(token->type==SEPARATOR&&strcmp(token->value,";")==0){
        Node *semi_colon_node=initialize_node(token->value,token->type);
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

//handle while statements
/*
  In nested scopes ,I don't want to push or pop from the code generation scope ,,cause really what we need is 
  to know the function offset size so that we can know how many bytes to allocate on the stack
  for the variables defined in there ,,but for the parsing stack ,I have to parse and pop from nested scopes
*/
Node *while_statement_generation(Node *node,Token **current_token_ptr){
   Token *token=*current_token_ptr;
   Node *while_statement_node=initialize_node(token->value,token->type);
   node->left=while_statement_node;
   token++;
   
   //at this point the current token should be an open parenthesis
   if(strcmp(token->value,"EOF")==0){
     end_of_tokens_error(token->line_num);
   }
    
   if(strcmp(token->value,"(")==0){
      Node *open_parens_node=initialize_node(token->value,token->type);
      while_statement_node->left=open_parens_node;
      token++;
      token++;
      //current token at this point should be an operator i.e == or < etc ,but it cannot be '='
       if(strcmp(token->value,"EOF")==0){
         end_of_tokens_error(token->line_num);
       }
       Node *op_node=initialize_node(token->value,token->type);
       if(token->type==OPERATOR && strcmp(token->value,"=")){
        open_parens_node->left=op_node;
       }else{
          missing_token_error("comparison operator",*(token-1));
       }
       
      token--;
      //current token at this point should be an identfier or an integer(LHS)
      if (token->type==IDENTIFIER || token->type==INT){
          if(token->type==IDENTIFIER && !check_variable(token->value)){
            fprintf(stderr, "\033[1;31mError:\033[0m Variable '%s' is not defined (line %zu)\n", token->value, token->line_num);
            exit(1);
          }
          Node *left_expr_node=initialize_node(token->value,token->type);
          op_node->left=left_expr_node;
          token++;
          token++;
         
          //we are at the right side(RHS) of the expression ,this should be an identifier or an integer
          if(strcmp(token->value,"EOF")==0) end_of_tokens_error(token->line_num);
          if(token->type ==IDENTIFIER|| token->type==INT){
             Node *right_expr_node=initialize_node(token->value,token->type);
             op_node->right=right_expr_node;
             token++;
             //This is the close parenthesis
             if(strcmp(token->value,"EOF")==0){
              end_of_tokens_error(token->line_num);
            }
               
              if(strcmp(token->value,")")==0){
                  Node *close_parens_node=initialize_node(token->value,token->type);
                  open_parens_node->right=close_parens_node;
                  token++;
                  //This should be open curly braces
                  if(strcmp(token->value,"EOF")==0){
                    end_of_tokens_error(token->line_num);
                  }
                  if(strcmp(token->value,"{")==0){
                    Table *scope_table=create_table();
                  
                     push_scope(&scope_stack,scope_table);
                     
                     Node *open_curly_node=initialize_node(token->value,token->type);
                     close_parens_node->left=open_curly_node;
                     node=open_curly_node;
                     token++;
                     
                     if(strcmp(token->value,"EOF")==0){
                      end_of_tokens_error(token->line_num);
                    }

                  
                     //at this point we are inside the block {here}
                     while(strcmp(token->value,"}")!=0){
                      if(token->type==IDENTIFIER){
                       
                        node=handle_variable_reassignment(node,&token);
                        
                      }else if (strcmp(token->value,"int")==0){
                        
                        node=create_variables(node,&token);
                       
                      }else if(strcmp(token->value,"while")==0){
                         node=while_statement_generation(node,&token);
                      }else if(strcmp(token->value,"if")==0){
                         node=if_statement_generation(node,&token);
                      }

                      // if the tokens come to an end without getting out of this loop then we had no } ,,throw an error.

                     }
                     if(strcmp(token->value,"EOF")==0){
                      end_of_tokens_error(token->line_num);
                    }
                      if(strcmp(token->value,"}")==0){
                        
                         
                         Node *close_curly_node=initialize_node(token->value,token->type);
                         close_parens_node->right=close_curly_node;
                         node=close_curly_node;
                         pop_scope(&scope_stack);
                         
                         token++;
                      }
                       
                     

                  }else{
                    missing_token_error(")",*(token-1));
                  }
              }else {
                missing_token_error(")",*(token-1));
              }
          }else{
            missing_token_error("integer literal or an identifier",*(token-1));

          }
      }else{
         missing_token_error("integer literal or an identifier",*(token-1));
      }

   }else{
      missing_token_error("(",*(token-1));
   }

   *current_token_ptr=token;

  return node;
}

// handle if-else statements
Node *if_statement_generation(Node *node,Token **current_token_ptr){
  Token *token=*current_token_ptr;
 
   Node *if_statement_node=initialize_node(token->value,token->type);
   node->left=if_statement_node;
   token++;
   
   //at this point the current token should be an open parenthesis
   if(strcmp(token->value,"EOF")==0){
     end_of_tokens_error(token->line_num);
   }
    
   if(strcmp(token->value,"(")==0){
      Node *open_parens_node=initialize_node(token->value,token->type);
      if_statement_node->left=open_parens_node;
      token++;
      token++;
      //current token at this point should be an operator i.e == or < etc ,but it cannot be '='
       if(strcmp(token->value,"EOF")==0){
         end_of_tokens_error(token->line_num);
       }
       Node *op_node=initialize_node(token->value,token->type);
       if(token->type==OPERATOR && strcmp(token->value,"=")){
        
        open_parens_node->left=op_node;
       }else{
          missing_token_error("comparison operator",*(token-1));
       }
       
      token--;
      //current token at this point should be an identfier or an integer(LHS)
      if (token->type==IDENTIFIER || token->type==INT){
          if(token->type==IDENTIFIER && !check_variable(token->value)){
            fprintf(stderr, "\033[1;31mError:\033[0m Variable '%s' is not defined (line %zu)\n", token->value, token->line_num);
            exit(1);
          }
          Node *left_expr_node=initialize_node(token->value,token->type);
          op_node->left=left_expr_node;
          token++;
          token++;
         
          //we are at the right side(RHS) of the expression ,this should be an identifier or an integer
          if(strcmp(token->value,"EOF")==0) end_of_tokens_error(token->line_num);
          if(token->type ==IDENTIFIER|| token->type==INT){
             Node *right_expr_node=initialize_node(token->value,token->type);
             op_node->right=right_expr_node;
             token++;
             //This is the close parenthesis
             if(strcmp(token->value,"EOF")==0){
              end_of_tokens_error(token->line_num);
            }
               
              if(strcmp(token->value,")")==0){
                  Node *close_parens_node=initialize_node(token->value,token->type);
                  open_parens_node->right=close_parens_node;
                  token++;
                  //This should be open curly braces
                  if(strcmp(token->value,"EOF")==0){
                    end_of_tokens_error(token->line_num);
                  }
                  if(strcmp(token->value,"{")==0){
                    //only push to parsing scope stack ,for reasons mentioned earlie
                    Table *scope_table=create_table();
            
                     push_scope(&scope_stack,scope_table);
                     
                     Node *open_curly_node=initialize_node(token->value,token->type);
                     close_parens_node->left=open_curly_node;
                     node=open_curly_node;
                     token++;
                     
                     if(strcmp(token->value,"EOF")==0){
                      end_of_tokens_error(token->line_num);
                    }

                  
                     //at this point we are inside the block {here}
                     while(strcmp(token->value,"}")!=0){
                      if(token->type==IDENTIFIER){
                      
                        node=handle_variable_reassignment(node,&token);
                        
                      }else if (strcmp(token->value,"int")==0){
                        
                        node=create_variables(node,&token);
                       
                      }else if(strcmp(token->value,"while")==0){
                        node=while_statement_generation(node,&token);
                      }else if(strcmp(token->value,"if")==0){
                         node=if_statement_generation(node,&token);
                      }
                     }
                       
                      
                      if(strcmp(token->value,"}")==0){
                        pop_scope(&scope_stack);
                         Node *close_curly_node=initialize_node(token->value,token->type);
                         close_parens_node->right=close_curly_node;
                         node=close_curly_node;
                         
                         token++;
                         if(strcmp(token->value,"else")==0 && token->type==KEYWORD){
                            Node *else_node=initialize_node(token->value,token->type);
                            if_statement_node->right=else_node;
                            token++;
                            //This is open curly '{'
                           
                            Table *scope_table=create_table();
                            push_scope(&scope_stack,scope_table);
                            Node *open_curly_node=initialize_node(token->value,token->type);
                            else_node->left=open_curly_node;
                            node=open_curly_node;
                            token++;
                          
                            while (strcmp(token->value,"}")!=0){
                           
                              //if it is a new variable we create a variabe,,if it is an identifier we update or reuse it
                              if(token->type==IDENTIFIER){
                                if(!check_variable(token->value)){
                                  fprintf(stderr, "\033[1;31mError:\033[0m Variable '%s' is not defined in the current scope (line %zu)\n", token->value, token->line_num);
                                  exit(1);
                                }
                                 node=handle_variable_reassignment(node,&token);
                                  
                              }else if(strcmp(token->value,"int")==0){
                                 
                                node=create_variables(node,&token);
                                
                              }else if(strcmp(token->value,"while")==0){
                                 node=while_statement_generation(node,&token);
                              }else if(strcmp(token->value,"if")==0){
                                 node=if_statement_generation(node,&token);
                              }
                             
                            }
                            //we have reached the closing curly brace,,end of scope
                            pop_scope(&scope_stack);
                            Node *close_curly_node=initialize_node(token->value,token->type);
                            else_node->right=close_curly_node;
                            node=close_curly_node;
                            token++;

                         }
                         
                      }
                  }else{
                    missing_token_error(")",*(token-1));
                  }
              }else {
                missing_token_error(")",*(token-1));
              }
          }else{
            missing_token_error("integer literal or an identifier",*(token-1));

          }
      }else{
         missing_token_error("integer literal or an identifier",*(token-1));
      }

   }else{
      missing_token_error("(",*(token-1));
   }

   *current_token_ptr=token;
  
   return node;

}



// function to parse tokens and create AST
Node *parser(Token *tokens) {
        //Initialize the two stacks
    init_scope_stack(&scope_stack);
    init_scope_stack(&code_gen_stack);
    Token *current_token=&tokens[0];

    Node *current=malloc(sizeof(Node));
    
    Node *left=malloc(sizeof(Node));
   
    Node *root=initialize_node("START",BEGINNING);
    //for functions
    function_table=create_function_table();
    //initialize a symbol table for the global scope
   
       /*
         The table should be stored in both parsing scope ,and code generation scope
         The scopes are literally similar ,the difference is ,the parsing scope will be used for parsing
         and the code generation scope will be used in code generation
         Since I cannot have one table pointed to by the two scopes(in this case a change in one table affects the other )
         I will have two tables ,in different memory locations
       */
      Table *global_scope_table=create_table();
      Table *global_codegen_table=create_table();
      push_scope(&scope_stack,global_scope_table);
      push_scope(&code_gen_stack,global_codegen_table);
      current=root;
    
    while(strcmp(current_token->value,"EOF")){

      switch(current_token->type){
         case KEYWORD:
           if (strcmp(current_token->value,"exit")==0){
           
               handle_exit_system(root,&current_token);
               
               break;
           }else if(strcmp(current_token->value,"int")==0){
               //so this can be a variable being created or a function being created..
               /*
                Instead of handling variable and function creation diffently in here
                Just let the program go to create_variables function then in there ,if the token after the identifier is '(' instead of '=' 
                call the create_function function 
               */

              
              
               Node *var_left=create_variables(current,&current_token);
               current=var_left;
               
              
               
           }else if (strcmp(current_token->value,"if")==0){
               //closed curly brace is what we will evaluate last and return it,,so it will be the current node
               Node *close_curly_node=if_statement_generation(current,&current_token);
               current=close_curly_node;
             
           }else if(strcmp(current_token->value,"while")==0){
             
               Node *while_node=while_statement_generation(current,&current_token); //close curly node is what this function returns 
               current=while_node;
              
              
               
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
//function to free allocated memory on the nodes
/*
 so I wanna do a dfs ,free memory starting from leaf nodes ,then the parent
*/
void free_nodes(Node *node){
   if(!node) return;

   free_nodes(node->left);
   free_nodes(node->right);
   free(node->value);
  //  free(node->type);
   free(node);
}
