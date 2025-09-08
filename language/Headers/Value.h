
#ifndef VALUE_H
#define VALUE_H

enum DataType {
    NullType,
    IntegerType,
    FloatType,
    StringType,
    BooleanType
};

struct Value {
    enum DataType Type;
    int* ValuePointer;
};

#endif