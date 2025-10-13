#include <stdio.h>
#include <stdbool.h>

#include "../Headers/Token.h"
#include "../Headers/ASTNode.h"
#include "../Headers/Value.h"
#include "../Headers/ErrorHandler.h"

#include "Arithmetic.c"
#include "TypeConverter.c"
#include "FunctionCaller.c"

struct Variable {
    char* Name;
    struct Value Value;
};

// Gets a variable from the variable list
struct Value GetVariable(char* Name, struct Variable** Variables, int* VariableCount) {

    for (int i = 0; i < *VariableCount; i++) {
        if (strcmp(Name, (*Variables)[i].Name) == 0) {
            return (*Variables)[i].Value;
        }
    }

    Error("Variable not initialized");
}

struct Variable ParseLine(struct ASTNode AST, struct Variable** Variables, int* VariableCount) {


    // Creates a Null variable containing a Null value
    struct Value Value;
    Value.Type = NullType;
    Value.ValuePointer = NULL;

    struct Variable Variable;
    Variable.Name = NULL;
    Variable.Value = Value;
    
    if (AST.Type == LeafNode) {
        
        if (AST.Token.TokenType == VariableToken) {
            Variable.Name = AST.Token.Text;
            struct Value Value = GetVariable(Variable.Name, Variables, VariableCount);
            Variable.Value = Value;
        }
        else {
            // Datatypes in the TokenTypes and DataTypes enums are aligned so this works
            Variable.Value.Type = AST.Token.TokenType;
            Variable.Value.ValuePointer = ConvertTokenToType(AST.Token.Text, AST.Token.TokenType);
        }
    }
    else if (AST.Type == AssigmentNode) {

        if (AST.ChildNodes[1].Type == EmptyNode) {
            Error("No value assigned to variable");
        }
        else if (AST.ChildNodes[1].Type == AssigmentNode) {
            Error("Invalid Sytnax");
        }

        Variable.Name = AST.ChildNodes[0].Token.Text;
        Variable.Value = ParseLine(AST.ChildNodes[1], Variables, VariableCount).Value;
    }
    else if (AST.Type == OperatorNode) {
        if (AST.ChildNodes[0].Type == EmptyNode) {
            Error("Operator missing left operand");
        }
        else if (AST.ChildNodes[1].Type == EmptyNode) {
            Error("Operator missing right operand");
        }
        struct Value LeftOperand = ParseLine(AST.ChildNodes[0], Variables, VariableCount).Value;
        struct Value RightOperand = ParseLine(AST.ChildNodes[1], Variables, VariableCount).Value;

        if (AST.FunctionType == MathematicalOperator) {
            Variable.Value = CalculateMath(LeftOperand, RightOperand, AST.Token.Text);
        }
        else if (AST.FunctionType == LogicalOperator) {
            Variable.Value = CalculateLogic(LeftOperand, RightOperand, AST.Token.Text);
        }
    }
    else if (AST.Type == SelectionNode) {
        if (AST.ChildNodes[0].Type == EmptyNode) {
            Error("Selection missing condition");
        }

        struct Value Condition = ParseLine(AST.ChildNodes[0], Variables, VariableCount).Value;

        if (Condition.Type != BooleanType) {
            Error("Selection condition must be a boolean");
        }

        Variable.Value = Condition;
        Variable.Value.Type = ConditionType;

    }
    else if (AST.Type == FunctionNode) {
        struct Value* FunctionArgs = malloc(AST.ChildNodeCount * sizeof(struct Value));

        for (int i = 0; i < AST.ChildNodeCount; i++) {
            FunctionArgs[i] = ParseLine(AST.ChildNodes[i], Variables, VariableCount).Value;
        }

        CallFunction(AST.Token.Text, FunctionArgs, AST.ChildNodeCount);
    }

    return Variable;
}

struct Value Parse(char* FileChars, struct ASTNode AST, struct Variable** Variables, int* VariableCount) {

    struct Variable Variable = ParseLine(AST, Variables, VariableCount);

    if (Variable.Name != NULL) {
        *Variables = realloc(*Variables, ((*VariableCount) + 1) * sizeof(struct Variable));
        (*Variables)[*VariableCount] = Variable;
        *VariableCount += 1;
    }

    return Variable.Value;
}