#include <stdio.h>

#include "../../Headers/Value.h"

int print(struct Value Value) {

    if (Value.Type == IntegerType) {
        printf("%d\n", *Value.ValuePointer);
    }
    else if (Value.Type == FloatType) {
        printf("%f\n", *(float*) Value.ValuePointer);
    }
    else if (Value.Type == StringType) {
        printf("%s\n", (char*) Value.ValuePointer);
    }
    else if (Value.Type == BooleanType) {
        if (*Value.ValuePointer) {
            printf("True\n");
        }
        else {
            printf("False\n");
        }
    }

    return 0;
}