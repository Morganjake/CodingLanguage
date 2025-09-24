#include <stdio.h>

#include "../Headers/Token.h"
#include "../Headers/ASTNode.h"
#include "../Headers/Value.h"

#include "../ErrorHandling/ErrorHandler.c"

struct Value CalculateMath(struct Value LeftOperand, struct Value RightOperand, char* Operator) {
	
    struct Value Result;
    Result.Type = NullType;
    Result.ValuePointer = NULL;

    // The value of a boolean is either 0 or 1 so they can be treated as an integer
    if (LeftOperand.Type == BooleanType) {
        LeftOperand.Type = IntegerType;
    }
    if (RightOperand.Type == BooleanType) {
        RightOperand.Type = IntegerType;
    }

    if (LeftOperand.Type == IntegerType && RightOperand.Type == IntegerType) {
        int LeftValue = *LeftOperand.ValuePointer;
        int RightValue = *RightOperand.ValuePointer;

        int* ResultPtr = malloc(sizeof(int));

        switch (Operator[0]) {
            case '+': *ResultPtr = LeftValue + RightValue; break;
            case '-': *ResultPtr = LeftValue - RightValue; break;
            case '*': *ResultPtr = LeftValue * RightValue; break;
            case '/': *ResultPtr = LeftValue / RightValue; break;
        }

        Result.Type = IntegerType;
        Result.ValuePointer = ResultPtr;
    }
    
    else if (LeftOperand.Type == FloatType && RightOperand.Type == FloatType) {
        float LeftValue = *(float*) LeftOperand.ValuePointer;
        float RightValue = *(float*) RightOperand.ValuePointer;

        float* ResultPtr = malloc(sizeof(float));

        switch (Operator[0]) {
            case '+': *ResultPtr = LeftValue + RightValue; break;
            case '-': *ResultPtr = LeftValue - RightValue; break;
            case '*': *ResultPtr = LeftValue * RightValue; break;
            case '/': *ResultPtr = LeftValue / RightValue; break;
        }

        Result.Type = FloatType;
        Result.ValuePointer = (int*) ResultPtr;
    }

    else if (LeftOperand.Type == IntegerType && RightOperand.Type == FloatType ||
        LeftOperand.Type == FloatType && RightOperand.Type == IntegerType) {

        float LeftValue;
        float RightValue;
        if (LeftOperand.Type == IntegerType && RightOperand.Type == FloatType) {
            LeftValue = (float) *LeftOperand.ValuePointer;
            RightValue = *(float*) RightOperand.ValuePointer;
        }
        else {
            LeftValue = *(float*) LeftOperand.ValuePointer;
            RightValue = (float) *RightOperand.ValuePointer;
        }
        float* ResultPtr = malloc(sizeof(float));

        switch (Operator[0]) {
            case '+': *ResultPtr = LeftValue + RightValue; break;
            case '-': *ResultPtr = LeftValue - RightValue; break;
            case '*': *ResultPtr = LeftValue * RightValue; break;
            case '/': *ResultPtr = LeftValue / RightValue; break;
        }

        Result.Type = FloatType;
        Result.ValuePointer = (int*) ResultPtr;
    }

    else if (LeftOperand.Type == StringType && RightOperand.Type == StringType) {

        // Only + is supported for strings
        if (Operator[0] == '+') {
            char* LeftValue = (char*) LeftOperand.ValuePointer;
            char* RightValue = (char*) RightOperand.ValuePointer;

            char* ResultPtr = malloc((strlen(LeftValue) + strlen(RightValue) + 1) * sizeof(char));
            strcpy(ResultPtr, LeftValue);
            strcat(ResultPtr, RightValue);

            Result.Type = StringType;
            Result.ValuePointer = (int*) ResultPtr;
        }
    }

    if (Result.ValuePointer == NULL) {
        Error("Invalid operator for operands");
    }

    return Result;
}

struct Value CalculateLogic(struct Value LeftOperand, struct Value RightOperand, char* Operator) {

    struct Value Result;
    Result.Type = BooleanType;
    Result.ValuePointer = NULL;

    // The value of a boolean is either 0 or 1 so they can be treated as an integer
    if (LeftOperand.Type == BooleanType) {
        LeftOperand.Type = IntegerType;
    }
    if (RightOperand.Type == BooleanType) {
        RightOperand.Type = IntegerType;
    }

    if (LeftOperand.Type == IntegerType && RightOperand.Type == IntegerType) {
        int LeftValue = *LeftOperand.ValuePointer;
        int RightValue = *RightOperand.ValuePointer;

        int* ResultPtr = malloc(sizeof(int));

        if (strcmp(Operator, "<") == 0) {
            if (LeftValue < RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, "<=") == 0) {
            if (LeftValue <= RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, ">") == 0) {
            if (LeftValue > RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, ">=") == 0) {
            if (LeftValue >= RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, "==") == 0) {
            if (LeftValue == RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }

        Result.ValuePointer = ResultPtr;
    }
    else if (LeftOperand.Type == FloatType && RightOperand.Type == FloatType) {
        float LeftValue = *(float*) LeftOperand.ValuePointer;
        float RightValue = *(float*) RightOperand.ValuePointer;

        int* ResultPtr = malloc(sizeof(int));

        if (strcmp(Operator, "<") == 0) {
            if (LeftValue < RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, "<=") == 0) {
            if (LeftValue <= RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, ">") == 0) {
            if (LeftValue > RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, ">=") == 0) {
            if (LeftValue >= RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, "==") == 0) {
            if (LeftValue == RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }

        Result.ValuePointer = ResultPtr;
    }

    else if (LeftOperand.Type == IntegerType && RightOperand.Type == FloatType ||
        LeftOperand.Type == FloatType && RightOperand.Type == IntegerType) {

        float LeftValue;
        float RightValue;
        if (LeftOperand.Type == IntegerType && RightOperand.Type == FloatType) {
            LeftValue = (float) *LeftOperand.ValuePointer;
            RightValue = *(float*) RightOperand.ValuePointer;
        }
        else {
            LeftValue = *(float*) LeftOperand.ValuePointer;
            RightValue = (float) *RightOperand.ValuePointer;
        }

        int* ResultPtr = malloc(sizeof(int));

        if (strcmp(Operator, "<") == 0) {
            if (LeftValue < RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, "<=") == 0) {
            if (LeftValue <= RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, ">") == 0) {
            if (LeftValue > RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, ">=") == 0) {
            if (LeftValue >= RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }
        else if (strcmp(Operator, "==") == 0) {
            if (LeftValue == RightValue) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }

        Result.ValuePointer = ResultPtr;
    }

    else if (LeftOperand.Type == StringType && RightOperand.Type == StringType) {

        char* LeftValue = (char*) LeftOperand.ValuePointer;
        char* RightValue = (char*) RightOperand.ValuePointer;

        int* ResultPtr = malloc(sizeof(int));

        if (strcmp(Operator, "==") == 0) {
            if (strcmp(LeftValue, RightValue) == 0) { *ResultPtr = 1; }
            else { *ResultPtr = 0; }
        }

        Result.ValuePointer = ResultPtr;
    }

    if (Result.ValuePointer == NULL) {
        Error("Invalid operator for operands");
    }
    
    return Result;
}