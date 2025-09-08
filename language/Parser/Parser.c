#include <stdio.h>
#include <stdbool.h>

#include "../Headers/Token.h"
#include "../Headers/ASTNode.h"
#include "../Headers/Value.h"

#include "Arithmetic.c"
#include "TypeConverter.c"
#include "FunctionCaller.c"

struct Variable {
    char* Name;
    struct Value Value;
};

// Gets a variable from the variable list
struct Value GetVariable(char* Name, struct Variable* Variables, int VariableCount) {
    
    for (int i = 0; i < VariableCount; i++) {
        if (strcmp(Name, Variables[i].Name) == 0) {
            return Variables[i].Value;
        }
    }
}

struct Variable ParseLine(struct ASTNode AST, struct Variable* Variables, int VariableCount) {

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
        Variable.Name = AST.ChildNodes[0].Token.Text;
        Variable.Value = ParseLine(AST.ChildNodes[1], Variables, VariableCount).Value;
    }
    else if (AST.Type == OperatorNode) {
        struct Value LeftOperand = ParseLine(AST.ChildNodes[0], Variables, VariableCount).Value;
        struct Value RightOperand = ParseLine(AST.ChildNodes[1], Variables, VariableCount).Value;

        Variable.Value = Calculate(LeftOperand, RightOperand, AST.Token.Text[0]);
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

void Parse(struct ASTNode* AST, int ASTNodeCount) {

    struct Variable* Variables = malloc(0);
    int VariableCount = 0;
    
    for (int i = 0; i < ASTNodeCount; i++) {
        struct Variable Variable = ParseLine(AST[i], Variables, VariableCount);

        if (Variable.Name != NULL) {
            Variables = realloc(Variables, (VariableCount + 1) * sizeof(struct Variable));
            Variables[VariableCount] = Variable;
            VariableCount++;
        }
    }
}