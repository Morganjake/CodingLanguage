#include <stdio.h>

#include "../language/Headers/Token.h"
#include "../language/Headers/ASTNode.h"

const char* TokenName(int TokenEnum) {
	
	switch (TokenEnum) {
		case StringToken: return "String";
		case IntegerToken: return "Integer";
		case FloatToken: return "Float";
		case BooleanToken: return "Boolean";
		case SelectionToken: return "Selection";
		case EndOfLineToken: return "EndOfLine";
		case VariableToken: return "Variable";
		case AssignmentToken: return "Assignment";
		case MathematicalOperatorToken: return "Mathematical Operator";
		case FunctionToken: return "Function";
		case BracketToken: return "Bracket";
	}
}

void PrintSpaces(int SpaceCount) {
	for (int i = 0; i < SpaceCount; i += 2) {
		printf("| ");
	}
}

void InnerObserveAST(struct ASTNode ASTNode, int SpaceCount) {

	PrintSpaces(SpaceCount);

	if (ASTNode.Type == LeafNode) {
		printf("Leaf Node: ");
		struct Token Token = ASTNode.Token;
		printf("Token Type: %s, Token Value: %s\n", TokenName(Token.TokenType), Token.Text);
	}
	else if (ASTNode.Type == AssigmentNode) {
		printf("Assigment Node\n");
		InnerObserveAST(ASTNode.ChildNodes[0], SpaceCount + 2);
		InnerObserveAST(ASTNode.ChildNodes[1], SpaceCount + 2);
	}
	else if (ASTNode.Type == OperatorNode) {
		printf("Operator Node\n");    
		InnerObserveAST(ASTNode.ChildNodes[0], SpaceCount + 2);
		PrintSpaces(SpaceCount + 2); printf("Operator: %s\n", ASTNode.Token.Text);
		InnerObserveAST(ASTNode.ChildNodes[1], SpaceCount + 2);
	}
	else if (ASTNode.Type == SelectionNode) {
		printf("Selection Node\n");
		PrintSpaces(SpaceCount + 2); printf("Condition:\n");
		InnerObserveAST(ASTNode.ChildNodes[0], SpaceCount + 4);
	}
	else if (ASTNode.Type == FunctionNode) {
		printf("Function Node\n");
		PrintSpaces(SpaceCount + 2); printf("Function Name: %s\n", ASTNode.Token.Text);

		for (int i = 0; i < ASTNode.ChildNodeCount; i++) {
			InnerObserveAST(ASTNode.ChildNodes[i], SpaceCount + 2);
		}
		
	}
	else if (ASTNode.Type == EmptyNode) {
		printf("Empty Node\n");   
	}
}

void ObserveAST(struct ASTNode AST) {

	printf("\nLine %d\n", GlobalLineNumber);
	InnerObserveAST(AST, 2);
}