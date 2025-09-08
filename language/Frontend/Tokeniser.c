#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../Headers/Token.h"

// Maximum size of a single token
#define MaxTokenBufferSize 1024

bool IsAlphabetic(char Char) {
	return (Char >= 65 && Char <= 90) || (Char >= 97 && Char <= 122);
}

bool IsNumeric(char Char) {
	return Char >= 48 && Char <= 57;
}

bool IsAlphanumeric(char Char) {
	return IsAlphabetic(Char) || IsNumeric(Char);
}

// Function to reset the token buffer and update the list of tokens
void UpdateTokens(struct Token** TokensPtr, char* TokenBuffer, int* TokenCountPtr,
	int* TokenBufferLocationPtr, enum TokenTypes TokenType) {

	*TokensPtr = realloc(*TokensPtr, (*TokenCountPtr + 1) * sizeof(struct Token));

	(*TokensPtr)[*TokenCountPtr].Text = malloc(strlen(TokenBuffer));
	strcpy((*TokensPtr)[*TokenCountPtr].Text, TokenBuffer);

	(*TokensPtr)[*TokenCountPtr].TokenType = TokenType;

	// Resets TokenBuffer
	memset(TokenBuffer, 0, MaxTokenBufferSize);

	*TokenBufferLocationPtr = 0;
	(*TokenCountPtr)++;
}

struct Tokens* Tokenize(char* FileChars) {

	int CharLocation = 0;

	char TokenBuffer[MaxTokenBufferSize];
	memset(TokenBuffer, 0, sizeof(TokenBuffer));
	int TokenBufferLocation = 0;

	struct Token *Tokens = malloc(0);
	int TokenCount = 0;

	// Iterates over the characters to turn them into tokens
	while (FileChars[CharLocation] != 0) {

		if (IsAlphabetic(FileChars[CharLocation])) {

			while (IsAlphanumeric(FileChars[CharLocation])) {
				TokenBuffer[TokenBufferLocation] = FileChars[CharLocation];
				TokenBufferLocation++;
				CharLocation++;
			}

			enum TokenTypes TokenType;
			
			if (FileChars[CharLocation] == '(') {
				TokenType = FunctionToken;
			}
			else {
				TokenType = VariableToken;
			}

			UpdateTokens(&Tokens, TokenBuffer, &TokenCount, &TokenBufferLocation, TokenType);
		}
		else if (IsNumeric(FileChars[CharLocation])) {
			
			while (IsNumeric(FileChars[CharLocation])) {
				TokenBuffer[TokenBufferLocation] = FileChars[CharLocation];
				TokenBufferLocation++;
				CharLocation++;
			}

			enum TokenTypes TokenType = IntegerToken;
			
			UpdateTokens(&Tokens, TokenBuffer, &TokenCount, &TokenBufferLocation, TokenType);

		}
		else if (FileChars[CharLocation] == '"') {
			CharLocation++;

			while (FileChars[CharLocation] != '"') {
				TokenBuffer[TokenBufferLocation] = FileChars[CharLocation];
				TokenBufferLocation++;
				CharLocation++;
			}

			enum TokenTypes TokenType = StringToken;
			UpdateTokens(&Tokens, TokenBuffer, &TokenCount, &TokenBufferLocation, TokenType);
			CharLocation++;
		}
		else if (strchr(";=+-*/()[]{},", FileChars[CharLocation]) != NULL) {
			enum TokenTypes TokenType;
			switch (FileChars[CharLocation]) {
				case ';': TokenType = EndOfLineToken; break;
				case '=': TokenType = AssignmentToken; break;
				case '+': TokenType = OperatorToken; break;
				case '-': TokenType = OperatorToken; break;
				case '*': TokenType = OperatorToken; break;
				case '/': TokenType = OperatorToken; break;
				case '(': TokenType = BracketToken; break;
				case ')': TokenType = BracketToken; break;
				case '[': TokenType = BracketToken; break;
				case ']': TokenType = BracketToken; break;
				case '{': TokenType = BracketToken; break;
				case '}': TokenType = BracketToken; break;
				case ',': TokenType = CommaToken; break;
			}
			TokenBuffer[0] = FileChars[CharLocation];
			UpdateTokens(&Tokens, TokenBuffer, &TokenCount, &TokenBufferLocation, TokenType);
			CharLocation++;
		}
		else {
			CharLocation++;
		}
	}

	struct Tokens* TokensStruct = malloc(sizeof(struct Tokens));
	TokensStruct->Tokens = Tokens;
	TokensStruct->TokenCount = TokenCount;

	return TokensStruct;
}