#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define NO_MATCH 1
#define PARTIAL_MATCH 2
#define FULL_MATCH 3
#define NUMBER_OF_VALID_WORDS 14855
#define NUMBER_OF_ANSWER_WORDS 2309

//IDS_STRING101 - IDS_STRING2409 are the IDS for valid answer words
//IDS_STRING2410 - IDS_STRING17264 are the IDS for valid words

bool ValidateGuess(char* guess, char** listOfWords);
char* GetGuess(HWND inputBoxes[6][5], DWORD currentLine);
bool CheckGuess(HWND inputBoxes[6][5], DWORD currentLine, char* answer);
void MoveToNextGuess(HWND inputBoxes[6][5], DWORD currentLine);
void ResetGame(HWND inputBoxes[6][5]);
char** LoadWords(bool readFromAnswersList);
char* GetRandomWord(char** listOfWords);