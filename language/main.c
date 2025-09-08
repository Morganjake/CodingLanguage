#include <stdio.h>
#include <Windows.h>

#include "Headers/Token.h"
#include "Headers/ASTNode.h"

#include "Frontend/FileReader.c"
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

int main(void) {
	
	FILE *File = fopen("../code/main.txt", "r");

	if (File == NULL) {
		printf("No main file found in code folder\n");
		return 0;
	}

	double StartBufferiseFile = GetTime();
	char* FileChars = BufferiseFile(File);
	double EndBufferiseFile = GetTime() - StartBufferiseFile;
	printf("Bufferised file in: %.4f ms\n", EndBufferiseFile);

	double StartTokenize = GetTime();
	struct Tokens* Tokens = Tokenize(FileChars);
	double EndTokenize = GetTime() - StartTokenize;
	printf("Tokenized text buffer in: %.4f ms\n", EndTokenize);
	
	int ASTNodeCount = 0;

	double StartCreateAST = GetTime();
	struct ASTNode* AST = CreateAST(Tokens->Tokens, Tokens->TokenCount, &ASTNodeCount);
	double EndCreateAST = GetTime() - StartCreateAST;
	printf("AST created in: %.4f ms\n", EndCreateAST);
	printf("Code output\n");
	printf("----------------------\n");

	double StartParse = GetTime();
	Parse(AST, ASTNodeCount);
	double EndParse = GetTime() - StartParse;
	printf("----------------------\n");
	printf("AST parsed in: %.4f ms\n", EndParse);
	
	printf("Code ran in %.4f ms\n", EndBufferiseFile + EndTokenize + EndCreateAST + EndParse);

	ObserveAST(AST, ASTNodeCount);

	return 0;
}