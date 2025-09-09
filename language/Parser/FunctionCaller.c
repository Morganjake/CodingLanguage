#include <stdio.h>

#include "../Headers/Value.h"
#include "Functions/Stdlib.c"

void CallFunction(char* FunctionName, struct Value* FunctionArgs, int ArgumentCount) {
    
    if (strcmp(FunctionName, "print") == 0) {
        print(FunctionArgs[0]);
    }
    else {
        Error("Function not defined");
    }
}