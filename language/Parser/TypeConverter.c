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
}