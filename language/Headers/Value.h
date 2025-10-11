
#ifndef VALUE_H
#define VALUE_H

enum DataType {
    NullType,
    IntegerType,
    FloatType,
    StringType,
    BooleanType,

    // Not a data type, but used to return an evaluated statement
    ConditionType
};

struct Value {
    enum DataType Type;
    int* ValuePointer;
};

#endif