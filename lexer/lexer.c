
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
      else if(strcmp(token->value,"do")==0) token->type=KEYWORD;
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

/*
  To get a comment ,we see // ,mark the rest of that line as comment.
  We see  ,mark the rest of contents as comments until we see a the end of comment terminator ()

*/

Token *generate_comment(char *current,int *current_index){
   Token *token=malloc(sizeof(Token));
   token->type=UNKNOWN;
   if(!token){
    printf("Failed to allocate\n");
    exit(1);
  }

   
   token->value=malloc(sizeof(char)*50);
   if(!token->value){
     printf("Failed to allocate\n");
     exit(1);
   }
   int token_index=0;
   
   if(current[(*current_index)+1]=='/'){
   
    while(current[*current_index]!='\n' && current[*current_index] !='\0'){
      token->value[token_index]=current[*current_index];
      token_index++;
      (*current_index)++;
   }
   }else if(current[(*current_index)+1]=='*'){
      //get the first two '/*'
      token->value[token_index]=current[*current_index];
      token_index++;
      (*current_index)++;
      
      token->value[token_index]=current[*current_index];
      token_index++;
      (*current_index)++;
      while(current[*current_index]!='*' && current[(*current_index)+1]!='/'){
        if(current[*current_index]=='\n'){
          token->value[token_index]=current[*current_index];
          token_index++;
          line_num++;
          (*current_index)++;
          continue;
        }
         token->value[token_index]=current[*current_index];
         token_index++;
         (*current_index)++;
        }
        
      token->value[token_index]=current[*current_index];
      token_index++;
      (*current_index)++;
     
      token->value[token_index]=current[*current_index];
    
   }
   token_index++;
   (*current_index)++;
   token->value[token_index]='\0';
   token->type=COMMENT;
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
        (current[*current_index] == '+'   && current[*current_index+1] == '+') ||
        (current[*current_index] == '-'   && current[*current_index+1] == '-') ||
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



/*
  since the tokens are enumerated as integers  i.e 1,2,3,4,,we wanna represent them as strings when printing them
  Just to make work easy.
*/

const char *token_type_to_string(TokenType type) {
  switch (type) {
    case BEGINNING: return "BEGINNING";
    case INT: return "INT";
    case KEYWORD: return "KEYWORD";
    case SEPARATOR: return "SEPARATOR";
    case OPERATOR: return "OPERATOR";
    case IDENTIFIER: return "IDENTIFIER";
    case STRING: return "STRING";
    case COMMENT: return "COMMENT";
    case END_OF_TOKENS: return "END_OF_TOKENS";
    default: return "UNKNOWN";
  }
}





Token *lexer(FILE *file){
    
     
     int current_index=0;
     /*
       We wanna move to the end of the file,
       We also want to get the file size ftell(file)
       We wanna allocate memory for the file contents ,+1 for null termination
       and finally want to move the pointer back to the start of the file
       Then start reading the file since we have allocated enough size for the file contents
       fread(memory we allocated,number of characters to read,file length, pointer to the file)
     */
    // Find the file length
    fseek(file, 0, SEEK_END);     
    int length = ftell(file);      
    rewind(file);                   
    char *current = malloc(length + 1);
     if (current == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        
    }

   
    fread(current, 1, length, file);
    current[length] = '\0';       
    /*
      We want to store the tokens in an array, the size of the token array will be dynamic
      In a way that if we fill up the contents in the allocated token array memory,we stretch the 
      tokens array size by 2,using realloc function
    */
    int number_of_tokens = 10;
    int tokens_size = 0;
    Token *tokens = malloc(sizeof(Token) * number_of_tokens);
    int tokens_index = 0;
    /*
      we wanna loop through the file contents we stored in current and classify them into tokens
      i.e keyword token,identifier token ,separator ,operator etc.. and the loop should terminated if we see a null terminator
      since we null terminated the string we stored in current
      We also want to skip new line characters and white spaces
    */
    while (current[current_index]!='\0'){
            tokens_size++;
             if(tokens_size >= number_of_tokens){
             number_of_tokens *= 2;
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
           if(curr=='/' && (current[current_index+1]=='/' || current[current_index+1]=='*')){
              token=generate_comment(current,&current_index);
              
           }else if (strchr("=+*-></!;(){},", curr)) {
            TokenType type = strchr(";(){},", curr) ? SEPARATOR : OPERATOR;
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

           /*
             At this point we wanna add the token into the tokens array
           */

           if (token) {
            tokens[tokens_index] = *token;
            tokens[tokens_index].value = strdup(token->value); 
            free(token->value); 
            free(token); 
            tokens_index++;
        }
    }
    
    /*
      Finally we want to know where our tokens array ends 
      EOF-value
      type-END_OF_TOKENS
    */
    tokens[tokens_index].value=strdup("EOF");
    tokens[tokens_index].type=END_OF_TOKENS;
    tokens[tokens_index].line_num=line_num;
    tokens_index++;
    
    /*
      clean up the allocated memory
    */
    
    free(current);
    fclose(file);

     return tokens;

     
}

void print_tokens(Token *tokens){
  
     Token *token=tokens;
     while(token->value){
      printf("TOKEN TYPE: %s , TOKEN VALUE: %s , on line: %zu\n",token_type_to_string(token->type),token->value,token->line_num);
      token++;
      
     }
        
        
}
