#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum length of a single line
#define MaxLineBufferSize 1024

// Takes in the file and turns it into a char buffer
char* BufferiseFile(FILE* File) {

	int FileBufferSize = 0;
	char LineBuffer[MaxLineBufferSize];

	// Iterates over the lines to get the size of the buffer
	while(fgets(LineBuffer, MaxLineBufferSize, *(&File))) {
		FileBufferSize += strlen(LineBuffer);
	}

	char *FileChars = calloc(FileBufferSize + 1, sizeof(char));
	
	for (int i = 0; i < FileBufferSize; i++) {
		FileChars[i] = ' ';
	}

	rewind(File);

	int BufferLocation = 0;

	// Iterates over the lines and adds it to a single array
	while(fgets(LineBuffer, MaxLineBufferSize, File)) {

		for (int i = 0; i < MaxLineBufferSize; i++) {
			FileChars[BufferLocation] = LineBuffer[i];
			BufferLocation++;
			
			if (LineBuffer[i] == 10 || LineBuffer[i] == 0) {
				break;
			}
		}
	}

	return FileChars;
}