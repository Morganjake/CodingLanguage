#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum length of a single line
#define MaxLineBufferSize 1024

// Takes in the file and turns it into a char buffer
char* BufferiseFile(FILE* File) {

	int FileBufferSize = 0;
	char LineBuffer[MaxLineBufferSize];

	char* FileChars = malloc(0);

	int BufferLocation = 0;

	// Iterates over the lines and adds it to a single array
	while(fgets(LineBuffer, MaxLineBufferSize, File)) {

		for (int i = 0; i < MaxLineBufferSize; i++) {
			FileChars = realloc(FileChars, (BufferLocation + 1) * sizeof(char));
			FileChars[BufferLocation] = LineBuffer[i];
			BufferLocation++;
			
			if (LineBuffer[i] == 10 || LineBuffer[i] == 0) {
				break;
			}
		}
	}

	return FileChars;
}