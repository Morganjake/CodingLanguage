#include <stdio.h>

#include "../../Headers/Value.h"

int print(struct Value Value) {

    if (Value.Type == IntegerType) {
        printf("%d\n", *Value.ValuePointer);
    }
    else if (Value.Type == StringType) {
        printf("%s\n", (char*) Value.ValuePointer);
    }

    return 0;
}