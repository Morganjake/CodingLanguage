#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../Headers/Token.h"
#include "../Headers/ASTNode.h"
#include "../Headers/ErrorHandler.h"

struct ASTNodeReturnValue {
	struct ASTNode ASTNode;
	int CurrentToken;
	bool Priority;
};

// Returns whether an operation should take precedance over another operation e.g. * before +
bool GetCalculatePriority(char Symbol1, char Symbol2) {
	return (Symbol1 + 2) % 5 > (Symbol2 + 2) % 5;
}

// Helper function to package all the necessary return values into one struct
struct ASTNodeReturnValue CreateReturnValue(struct ASTNode ASTNode, int CurrentToken, bool Priority) {
	struct ASTNodeReturnValue ReturnValue;
	ReturnValue.ASTNode = ASTNode;
	ReturnValue.CurrentToken = CurrentToken;
	ReturnValue.Priority = Priority;
	return ReturnValue;
}

struct ASTNodeReturnValue CreateASTNode(struct Token* Tokens, int TokenCount, int CurrentToken) {

	// Creates an initial empty node
	struct ASTNode ASTNode;
	ASTNode.ChildNodes = malloc(0);
	ASTNode.ChildNodeCount = 0;
	ASTNode.Type = EmptyNode;
	ASTNode.FunctionType = None;

	bool Priority = false;

	while (CurrentToken < TokenCount) {
		
		struct Token Token = Tokens[CurrentToken];

		if (Token.TokenType == EndOfLineToken) {
			return CreateReturnValue(ASTNode, CurrentToken, Priority);
		}
		else if (Token.TokenType == IntegerToken || Token.TokenType == FloatToken || Token.TokenType == StringToken || 
			Token.TokenType == BooleanToken || Token.TokenType == VariableToken) {
				
			if (ASTNode.Type != EmptyNode) {
				Error("Syntax Error");
			}

			ASTNode.Type = LeafNode;
			ASTNode.Token = Token;

			CurrentToken++;
		}
		// Creates a new node and assigns the current ASTNode to one of its child nodes as the current
		// ASTNode should be a variable name, and it also creates a node out of everything that falls
		// to the right of the token and assigns that to another child
		else if (Token.TokenType == AssignmentToken) {

			struct ASTNode AssignmentASTNode;
			AssignmentASTNode.Type = AssigmentNode;

			struct ASTNodeReturnValue ReturnValue = CreateASTNode(Tokens, TokenCount, CurrentToken + 1);

			AssignmentASTNode.ChildNodes = malloc(2 * sizeof(struct ASTNode));
			AssignmentASTNode.ChildNodes[0] = ASTNode;
			AssignmentASTNode.ChildNodes[1] = ReturnValue.ASTNode;

			ASTNode = AssignmentASTNode;

			CurrentToken = ReturnValue.CurrentToken;
		}
		// Creates a new node and assigns the current ASTNode to one of its child nodes. It then checks
		// to see whether the returned node is an operator node and if its operator should take
		// precedence over the new nodes operator
		else if (Token.TokenType == MathematicalOperatorToken) {

			struct ASTNode OperatorASTNode;
			OperatorASTNode.Type = OperatorNode;
			OperatorASTNode.FunctionType = MathematicalOperator;

			OperatorASTNode.ChildNodes = malloc(2 * sizeof(struct ASTNode));
			OperatorASTNode.ChildNodes[0] = ASTNode;
			OperatorASTNode.Token = Token;

			struct ASTNodeReturnValue ReturnValue = CreateASTNode(Tokens, TokenCount, CurrentToken + 1);
			// Current AST node takes less precedence
			if (ReturnValue.ASTNode.Type == OperatorNode && !ReturnValue.Priority &&
				(GetCalculatePriority(Token.Text[0], ReturnValue.ASTNode.Token.Text[0]) ||
				ReturnValue.ASTNode.FunctionType == LogicalOperator)) {
				
				// Sets the second child to the first child of the return value's ASTNode then sets
				// itself and the first child of the return value's ASTNode
				OperatorASTNode.ChildNodes[1] = ReturnValue.ASTNode.ChildNodes[0];
				ReturnValue.ASTNode.ChildNodes[0] = OperatorASTNode;
				ASTNode = ReturnValue.ASTNode;
			}
			else {
				OperatorASTNode.ChildNodes[1] = ReturnValue.ASTNode;
				ASTNode = OperatorASTNode;
			}

			CurrentToken = ReturnValue.CurrentToken;
		}
		else if (Token.TokenType == LogicalOperatorToken) {

			struct ASTNode OperatorASTNode;
			OperatorASTNode.Type = OperatorNode;
			OperatorASTNode.FunctionType = LogicalOperator;

			OperatorASTNode.ChildNodes = malloc(2 * sizeof(struct ASTNode));
			OperatorASTNode.ChildNodes[0] = ASTNode;
			OperatorASTNode.Token = Token;

			struct ASTNodeReturnValue ReturnValue = CreateASTNode(Tokens, TokenCount, CurrentToken + 1);

			if (ReturnValue.ASTNode.Type == OperatorNode && !ReturnValue.Priority &&
				ReturnValue.ASTNode.FunctionType == LogicalOperator) {

				OperatorASTNode.ChildNodes[1] = ReturnValue.ASTNode.ChildNodes[0];
				ReturnValue.ASTNode.ChildNodes[0] = OperatorASTNode;
				ASTNode = ReturnValue.ASTNode;
			}
			else {
				OperatorASTNode.ChildNodes[1] = ReturnValue.ASTNode;
				ASTNode = OperatorASTNode;
			}

			CurrentToken = ReturnValue.CurrentToken;
		}
		else if (Token.TokenType == BracketToken) {

			if (Token.Text[0] == '(') {

				if (ASTNode.Type != EmptyNode) {
					Error("Syntax Error");
				}
				
				struct ASTNodeReturnValue ReturnValue = CreateASTNode(Tokens, TokenCount, CurrentToken + 1);
				
				ASTNode = ReturnValue.ASTNode;
				CurrentToken = ReturnValue.CurrentToken;
				Priority = true;
			}
			else if (Token.Text[0] == ')') {
				// We return CurrentToken + 1 so that it skips the ')' when we return 
				return CreateReturnValue(ASTNode, CurrentToken + 1, Priority);
			}
		}
		else if (Token.TokenType == FunctionToken) {

			if (ASTNode.Type != EmptyNode) {
				Error("Syntax Error");
			}

			ASTNode.Type = FunctionNode;
			ASTNode.Token = Token;

			CurrentToken += 2;

			bool MoreArguments = true;

			while (MoreArguments) {
				struct ASTNodeReturnValue ReturnValue = CreateASTNode(Tokens, TokenCount, CurrentToken);

				ASTNode.ChildNodes = realloc(ASTNode.ChildNodes, (ASTNode.ChildNodeCount + 1) * sizeof(struct ASTNode));
				ASTNode.ChildNodes[ASTNode.ChildNodeCount] = ReturnValue.ASTNode;
				ASTNode.ChildNodeCount += 1;

				CurrentToken = ReturnValue.CurrentToken;

				if (!ReturnValue.Priority) {
					MoreArguments = false;
				}
			}
		}
		else if (Token.TokenType == CommaToken) {
			return CreateReturnValue(ASTNode, CurrentToken + 1, true);
		}
		else {
			CurrentToken++;
		}

	}

	return CreateReturnValue(ASTNode, CurrentToken, false);
}

struct ASTNode* CreateAST(struct Token* Tokens, int TokenCount, int* ASTNodeCount) {

	int CurrentToken = 0;
	struct ASTNode* AST = malloc(0);

	while (CurrentToken < TokenCount) {

		struct ASTNodeReturnValue ReturnValue = CreateASTNode(Tokens, TokenCount, CurrentToken);

        AST = realloc(AST, ((*ASTNodeCount) + 1) * sizeof(struct ASTNode));
        AST[*ASTNodeCount] = ReturnValue.ASTNode;
        (*ASTNodeCount)++;

        CurrentToken = ReturnValue.CurrentToken + 1;
	}

	return AST;
}