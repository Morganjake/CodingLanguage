#include <stdio.h>

#include "../Headers/Token.h"
#include "../Headers/ASTNode.h"
#include "../Headers/Value.h"

struct Value Calculate(struct Value LeftOperand, struct Value RightOperand, char Operator) {
	
    struct Value Result;
    Result.Type = NullType;
    Result.ValuePointer = NULL;

    if (LeftOperand.Type == IntegerType && RightOperand.Type == IntegerType) {
        int LeftValue = *LeftOperand.ValuePointer;
        int RightValue = *RightOperand.ValuePointer;

        int* ResultPtr = malloc(sizeof(int));

        switch (Operator) {
            case '+': *ResultPtr = LeftValue + RightValue; break;
            case '-': *ResultPtr = LeftValue - RightValue; break;
            case '*': *ResultPtr = LeftValue * RightValue; break;
            case '/': *ResultPtr = LeftValue / RightValue; break;
        }

        Result.Type = IntegerType;
        Result.ValuePointer = ResultPtr;
    }

    return Result;
}