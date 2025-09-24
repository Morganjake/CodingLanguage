
#ifndef TOKENTYPES_H
#define TOKENTYPES_H

enum TokenTypes {
	NullToken,
	IntegerToken,
	FloatToken,
	StringToken,
	BooleanToken,
	KeywordToken,
	EndOfLineToken,
	VariableToken,
	AssignmentToken,
	MathematicalOperatorToken,
	LogicalOperatorToken,
	BooleanOperatorToken,
	BracketToken,
	FunctionToken,
	CommaToken
};

struct Token {
	enum TokenTypes TokenType;
	char* Text;
};

struct Tokens {
	struct Token* Tokens;
	int TokenCount;
};

#endif