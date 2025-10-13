#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "Headers/Token.h"
#include "Headers/ASTNode.h"
#include "Headers/Value.h"

#include "Frontend/Tokenizer.c"
#include "Frontend/AbstractSyntaxTree.c"
#include "Parser/Parser.c"
#include "../debugging/CodeViewer.c"

// Maximum length of a single line
#define MaxLineBufferSize 1024

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

struct Value RunLine(char* LineChars, struct Variable** Variables, int* VariableCount) {

	struct Tokens* Tokens = Tokenize(LineChars);
	
	int ASTNodeCount = 0;
	
	struct ASTNode* AST = CreateAST(Tokens->Tokens, Tokens->TokenCount, &ASTNodeCount);
	
	ObserveAST(AST, ASTNodeCount);

	return Parse(LineChars, AST, ASTNodeCount, Variables, VariableCount);
}

void RunProgram() {
	
	FILE *File = fopen("../code/main.txt", "r");

	if (File == NULL) {
		printf("No main file found in code folder\n");
		return;
	}

	double StartProgramTime = GetTime();

	GlobalLineNumber = 1;

	char LineBuffer[MaxLineBufferSize];
	int BufferLocation = 0;

    struct Variable* Variables = malloc(0);
    int VariableCount = 0;

	char* FileChars = malloc(0);
	GlobalLine = malloc(0);

	// Iterates over the lines and adds it to a single array
	while(fgets(LineBuffer, MaxLineBufferSize, File)) {

		for (int i = 0; i < MaxLineBufferSize; i++) {
			
			FileChars = realloc(FileChars, (BufferLocation + 1) * sizeof(char));
			FileChars[BufferLocation] = LineBuffer[i];

			GlobalLine = realloc(GlobalLine, (BufferLocation + 1) * sizeof(char));
			GlobalLine[BufferLocation] = LineBuffer[i];

			BufferLocation++;

			if (LineBuffer[i] == 59 || LineBuffer[i] == 123) {
				printf("'%s'\n", FileChars);
				struct Value Value = RunLine(FileChars, &Variables, &VariableCount);
				free(FileChars);
				free(GlobalLine);
				BufferLocation = 0;
				GlobalLineNumber++;

				if (Value.Type == ConditionType && *Value.ValuePointer == 0) {
					// Skips to the end of the block
					int OpenBrackets = 1;
					while (OpenBrackets > 0) {
						fgets(LineBuffer, MaxLineBufferSize, File);
						GlobalLineNumber++;

						for (int i = 0; i < MaxLineBufferSize; i++) {

							if (LineBuffer[i] == 123) {
								OpenBrackets++;
							}
							else if (LineBuffer[i] == 125) {
								OpenBrackets--;
							}

							if (LineBuffer[i] == 10 || LineBuffer[i] == 0) {
								break;
							}
						}
					}
				}

				FileChars = malloc(0);
				GlobalLine = malloc(0);
			}
			
			if (LineBuffer[i] == 10 || LineBuffer[i] == 0) {
				break;
			}
		}
	}

	printf("Program ran in: %.4f ms\n", GetTime() - StartProgramTime);
	fclose(File);
}

int main(void) {
	
	for (int i = 0; i < 1; i++) {
		RunProgram();
	}

	return 0;
}