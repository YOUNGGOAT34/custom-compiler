#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "lexer.h"
#include "parser.h"

int main(){
    FILE *file=fopen("../test.g","r");
     if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    Token *tokens=lexer(file);
    
    Node *root=createNode("aaa");
    Node *left=createNode("bbb");
    Node *right=createNode("ccc");

    root->left=left;
    root->right=right;
    preorderTraversal(root);
    free(root);
    free(left);
    free(right);

    


   

    return 0;

}


// echo "# custom-compiler" >> README.md
// git init
// git add README.md
// git commit -m "first commit"
// git branch -M main
// git remote add origin https://github.com/YOUNGGOAT34/custom-compiler.git
// git push -u origin main