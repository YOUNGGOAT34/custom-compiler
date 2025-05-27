
#include "lexer.h"


size_t line_num=1;

Token *generate_keyword_identifier(char *current,int *current_index){
      Token *token=(Token *)malloc(sizeof(Token));
      token->value=malloc(sizeof(char)*10);
      int index=0;
      while(isalnum(current[*current_index])&&current[*current_index]!='\0'){
            token->value[index]=current[*current_index];
            index++;
            (*current_index)++;
      }

      token->value[index]='\0';

      if(strcmp(token->value,"exit")==0) token->type=KEYWORD;
      else if(strcmp(token->value,"if")==0) token->type=KEYWORD;
      else if(strcmp(token->value,"String")==0) token->type=KEYWORD;
      else if(strcmp(token->value,"int")==0) token->type=KEYWORD;
      else if(strcmp(token->value,"for")==0)  token->type=KEYWORD;
      else if(strcmp(token->value,"while")==0) token->type=KEYWORD;
      else if(strcmp(token->value,"else")==0) token->type=KEYWORD;
      else if(strcmp(token->value,"return")==0) token->type=KEYWORD;
      else token->type=IDENTIFIER;
      
      
      token->line_num=line_num;

      return token;
}

Token *generate_string(char *current,int *current_index){
    Token *token=(Token *)malloc(sizeof(Token));
    token->value=malloc(sizeof(char)*255);
    (*current_index)++;//skip the opening quote
    int index=0;

    while(current[*current_index]!='"'){
       token->value[index++]=current[*current_index];
       (*current_index)++;
    }
     
    token->value[index]='\0';

    if(current[*current_index]=='"'){
        (*current_index)++;
    }

    token->type=STRING;
    token->line_num=line_num;
    return token;

}


Token  *generate_number(char *current,int *current_index){
     
    Token *token = (Token *)malloc(sizeof(Token));
    token->value = (char *)malloc(sizeof(char) * 10);
     
     int index=0;

     while(isdigit(current[*current_index])){
          token->value[index]=current[*current_index];
          index++;
          (*current_index)++;
     }
     token->value[index]='\0';

    
 
 
  token->type=INT;
 
  token->line_num=line_num;

  
 
  return token;   
}


Token *generate_separator_operator(char *current,int *current_index,TokenType type){
     Token *token=malloc(sizeof(Token));
     token->type=UNKNOWN;
     token->value=malloc(sizeof(char)*2);
     if((current[*current_index] == '=' && current[*current_index + 1] == '=') || 
        (current[*current_index] == '!' && current[*current_index + 1] == '=') || 
        (current[*current_index] == '<' && current[*current_index + 1] == '=') || 
        (current[*current_index] == '>' && current[*current_index + 1] == '=')){
        token->value[0]=current[*current_index];
        
        token->value[1]=current[*current_index+1];
        token->value[2]='\0';
         token->type=type;
        token->line_num=line_num;
        (*current_index)+=2;
        return token;
     }
    token->value[0]=current[*current_index];
    token->value[1]='\0';
    (*current_index)++;
     
    
     token->type=type;
     token->line_num=line_num;

     return token;

}



const char *token_type_to_string(TokenType type) {
  switch (type) {
    case BEGINNING: return "BEGINNING";
    case INT: return "INT";
    case KEYWORD: return "KEYWORD";
    case SEPARATOR: return "SEPARATOR";
    case OPERATOR: return "OPERATOR";
    case IDENTIFIER: return "IDENTIFIER";
    case STRING: return "STRING";
    
    case END_OF_TOKENS: return "END_OF_TOKENS";
    default: return "UNKNOWN";
  }
}



Token *lexer(FILE *file){
    
     
     int current_index=0;
     
    // Find the file length
    fseek(file, 0, SEEK_END);       // Move to the end of the file
    int length = ftell(file);       // Get the current position (file size)
    rewind(file);                   // Move the file pointer back to the beginning
   char *current = malloc(length + 1);// Allocate memory for the file contents (+1 for null-terminator)
     if (current == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        
    }

    // Read the file contents
    fread(current, 1, length, file);
    current[length] = '\0';         // Null-terminate the string

    int number_of_tokens = 12;
    int tokens_size = 0;
    Token *tokens = malloc(sizeof(Token) * number_of_tokens);
    int tokens_index = 0;

    //go through the characters stored in current
    
    while (current[current_index]!='\0'){
            tokens_size++;
             if(tokens_size >= number_of_tokens){
             number_of_tokens *= 1.5;
              tokens = realloc(tokens, sizeof(Token) * number_of_tokens);
            }

            char curr=current[current_index];
            
            if(current[current_index]=='\n'){
                line_num++;
                current_index++;
                continue;
            }

            if (isspace(curr)) {
                 current_index++; 
                 continue; 
                 }
            
            Token *token = NULL;
          
           if (strchr("=+*-></!;(){}", curr)) {
            TokenType type = strchr(";(){}", curr) ? SEPARATOR : OPERATOR;
            token = generate_separator_operator(current, &current_index, type);
             }else if(isdigit(curr)){
               token=generate_number(current,&current_index);
               
           }else if(curr=='"'){
            token=generate_string(current,&current_index);
                
           }else if(isalpha(curr)){
               token=generate_keyword_identifier(current,&current_index);
           }else{
              fprintf(stderr, "Unrecognized character '%c' on line %zu\n", curr, line_num);
             current_index++;
             continue;
           }

           if (token) {
            tokens[tokens_index] = *token;
            tokens[tokens_index].value = strdup(token->value); // Make a separate copy of the value
            free(token->value); // Free the original value
            free(token); // Free the token struct itself
            tokens_index++;
        }
    }
    
    
    tokens[tokens_index].value=strdup("EOF");
    tokens[tokens_index].type=END_OF_TOKENS;
    tokens[tokens_index].line_num=line_num;
    tokens_index++;
    
    // Clean up
    
    free(current);
    fclose(file);

     return tokens;

     
}

void print_tokens(Token *tokens){
    int index=0;
     Token *token=tokens;
     while(token->value){
      printf("TOKEN TYPE: %s , TOKEN VALUE: %s , on line: %zu\n",token_type_to_string(tokens[index].type),token->value,token->line_num);
      token++;
     }
        
        
  

    
}

