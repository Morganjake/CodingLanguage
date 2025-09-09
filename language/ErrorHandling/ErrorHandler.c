#include <stdio.h>

#include <windows.h>

int GlobalLineNumber = 0;
char* GlobalLine = NULL;

void Error(char* ErrorMessage) {
    // Windows specific code to change console text colour
    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
    GetConsoleScreenBufferInfo(Console, &ConsoleInfo);
    WORD DefaultConsoleLook = ConsoleInfo.wAttributes;
    SetConsoleTextAttribute(Console, FOREGROUND_RED);

    printf("Line %d: ", GlobalLineNumber, GlobalLine);

    int GlobalLineIndex = 0;
    while (GlobalLine[GlobalLineIndex] != '\0' && GlobalLine[GlobalLineIndex] != '\n') {
        printf("%c", GlobalLine[GlobalLineIndex]);
        GlobalLineIndex++;
    }   

    printf("\nError: %s\n", ErrorMessage);
    SetConsoleTextAttribute(Console, DefaultConsoleLook);
    exit(1);
}