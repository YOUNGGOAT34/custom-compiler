#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "parser.h"
#include "lexer.h"



Node *createNode(char *val){
    Node *newNode=(Node*)malloc(sizeof(Node));
    newNode->data=val;
    newNode->left=NULL;
    newNode->right=NULL;

    return newNode;
}

// Preorder traversal function (Root, Left, Right)
void preorderTraversal(Node* root) {
    if (root == NULL) {
        return;
    }
    
    // Visit the root node
    printf("%s ", root->data);
    
    // Traverse the left subtree
    preorderTraversal(root->left);
    
    // Traverse the right subtree
    preorderTraversal(root->right);
}



void parser(Token *tokens) {
    
     
     
    
    
}
