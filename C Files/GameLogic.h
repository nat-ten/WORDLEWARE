#pragma once
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define NO_MATCH 1
#define PARTIAL_MATCH 2
#define FULL_MATCH 3
#define NUMBER_OF_VALID_WORDS 14855
#define NUMBER_OF_ANSWER_WORDS 2309

bool ValidateGuess(char* guess, char** listOfWords);
char* GetGuess(HWND inputBoxes[6][5], DWORD currentLine);
bool CheckGuess(HWND inputBoxes[6][5], DWORD currentLine, char* answer);
void MoveToNextGuess(HWND inputBoxes[6][5], DWORD currentLine);
void ResetGame(HWND inputBoxes[6][5]);
char** LoadValidWords(FILE* fileOfWords, bool readFromValidAnswersList);
char* GetRandomWord(char** listOfWords);