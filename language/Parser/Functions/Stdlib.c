#include <stdio.h>

#include "../../Headers/Value.h"

int print(struct Value Value) {

    if (Value.Type == IntegerType) {
        printf("%d\n", *Value.ValuePointer);
    }

    return 0;
}