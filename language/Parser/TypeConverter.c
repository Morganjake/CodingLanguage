#include <stdio.h>

#include "../Headers/Token.h"
#include "../Headers/ASTNode.h"
#include "../Headers/Value.h"


// Turns the text of a token into a value token's type then returns a pointer to it
int* ConvertTokenToType(char* Text, enum TokenTypes TokenType) {

    if (TokenType == IntegerToken) {
        int* ValuePtr = malloc(sizeof(int));
        *ValuePtr = atoi(Text);
        return ValuePtr;
    }
    else if  (TokenType == FloatToken) {
        float* ValuePtr = malloc(sizeof(float));
        *ValuePtr = atof(Text);
        return (int*) ValuePtr;
    }
    else if (TokenType == StringToken) {
        char* ValuePtr = malloc((strlen(Text) + 1) * sizeof(char));
        strcpy(ValuePtr, Text);
        return (int*) ValuePtr;
    }
    else if (TokenType == BooleanToken) {
        int* ValuePtr = malloc(sizeof(int));
        if (strcmp(Text, "True") == 0) {
            *ValuePtr = 1;
        }
        else {
            *ValuePtr = 0;
        }
        return ValuePtr;
    }
}