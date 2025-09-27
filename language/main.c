#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "Headers/Token.h"
#include "Headers/ASTNode.h"

#include "Frontend/Tokeniser.c"
#include "Frontend/AbstractSyntaxTree.c"
#include "Parser/Parser.c"
#include "../debugging/CodeViewer.c"

double GetTime() {
	static LARGE_INTEGER Frequency;
    static int Initialized = 0;
    LARGE_INTEGER Counter;

    if (!Initialized) {
        QueryPerformanceFrequency(&Frequency);
        Initialized = 1;
    }

    QueryPerformanceCounter(&Counter);
    return (double) Counter.QuadPart * 1000.0 / (double)Frequency.QuadPart;
}

// Maximum length of a single line
#define MaxLineBufferSize 1024

void Interpret(char* LineChars) {

	double StartTokenize = GetTime();
	struct Tokens* Tokens = Tokenize(LineChars);
	double EndTokenize = GetTime() - StartTokenize;
	
	int ASTNodeCount = 0;

	double StartCreateAST = GetTime();
	struct ASTNode* AST = CreateAST(Tokens->Tokens, Tokens->TokenCount, &ASTNodeCount);
	double EndCreateAST = GetTime() - StartCreateAST;

	double StartParse = GetTime();
	Parse(LineChars, AST, ASTNodeCount);
	double EndParse = GetTime() - StartParse;

	ObserveAST(AST, ASTNodeCount);
}

int main(void) {
	
	FILE *File = fopen("../code/main.txt", "r");

	if (File == NULL) {
		printf("No main file found in code folder\n");
		return 0;
	}

	double StartProgramTime = GetTime();

	GlobalLineNumber = 1;

	char LineBuffer[MaxLineBufferSize];
	
	int BufferLocation = 0;

	// Iterates over the lines and adds it to a single array
	while(fgets(LineBuffer, MaxLineBufferSize, File)) {

		char* FileChars = malloc(0);
		GlobalLine = malloc(0);

		for (int i = 0; i < MaxLineBufferSize; i++) {
			
			FileChars = realloc(FileChars, (BufferLocation + 1) * sizeof(char));
			FileChars[BufferLocation] = LineBuffer[i];

			GlobalLine = realloc(GlobalLine, (BufferLocation + 1) * sizeof(char));
			GlobalLine[BufferLocation] = LineBuffer[i];

			BufferLocation++;

			if (LineBuffer[i] == 59) {
				Interpret(FileChars);
				free(FileChars);
				free(GlobalLine);
				BufferLocation = 0;
				GlobalLineNumber++;
				break;
			}
			
			if (LineBuffer[i] == 10 || LineBuffer[i] == 0) {
				break;
			}
		}
	}

	printf("Program ran in: %.4f ms\n", GetTime() - StartProgramTime);
	fclose(File);

	return 0;
}