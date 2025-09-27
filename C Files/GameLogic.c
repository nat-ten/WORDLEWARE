#include "GameLogic.h"

bool ValidateGuess(char* guess, char** listOfWords) { //check if guess is a valid word from word list
    for (DWORD i = 0; i < NUMBER_OF_VALID_WORDS; i++) {
        for (DWORD j = 0; j < 5; j++) {
            if (!IsCharAlpha(guess[j])) { //no need to check if guess contains a unicode character, wcsctombs in GetGuess() would remove it if it did
                return FALSE;
            }

            else {
                if (guess[j] != listOfWords[i][j]) {
                    break;
                }

                else if (j == 4) {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

char* GetGuess(HWND inputBoxes[6][5], DWORD currentLine) { //get each character from each textbox for current guess and return it as a char*
    char* guess;
    guess = calloc(6, sizeof(char));

    char buffer[2] = { 0 };
    WCHAR input[2] = { 0 };

    for (DWORD i = 0; i < 5; i++) {
        GetWindowText(inputBoxes[currentLine][i], input, 2); //get one letter at a time
        wcstombs(buffer, input, sizeof(input));

        if (buffer[0] >= 'A' && buffer[0] <= 'Z') {
            CharLower(buffer);
        }

        strcat(guess, buffer);
    }
    return guess;
}

bool CheckGuess(HWND inputBoxes[6][5], DWORD currentLine, char* answer) { //determine which characters are in answer and change ID of textboxes to a number to identify how to colour them
    bool correctAnswer = FALSE;
    bool flags[5] = { FALSE };

    char* guess = { 0 };
    guess = GetGuess(inputBoxes, currentLine);

    for (DWORD i = 0; i < 5; i++) { //check if guess is an exact match
        if (guess[i] != answer[i]) {
            correctAnswer = FALSE;
            break;
        }

        if (i == 4) {
            SetWindowLongPtr(inputBoxes[currentLine][i], GWLP_ID, FULL_MATCH); //set textbox to green
            correctAnswer = TRUE;
            return correctAnswer;
        }

        else {
            SetWindowLongPtr(inputBoxes[currentLine][i], GWLP_ID, FULL_MATCH);
        }
    }

    for (DWORD i = 0; i < 5; i++) {
        if (guess[i] == answer[i]) {
            SetWindowLongPtr(inputBoxes[currentLine][i], GWLP_ID, FULL_MATCH);
            flags[i] = TRUE;
        }

        else {
            bool partialMatch = FALSE;
            for (DWORD j = 0; j < 5; j++) {
                if (guess[i] == answer[j] && guess[i] != guess[j] && !flags[j]) {
                    flags[j] = TRUE;
                    partialMatch = TRUE;
                    SetWindowLongPtr(inputBoxes[currentLine][i], GWLP_ID, PARTIAL_MATCH);
                    break;
                }
            }

            if (!partialMatch) {
                SetWindowLongPtr(inputBoxes[currentLine][i], GWLP_ID, NO_MATCH);
            }
        }
    }

    return correctAnswer;
}

void MoveToNextGuess(HWND inputBoxes[6][5], DWORD currentLine) { //increment current guess and set previous guess textboxes to read only
    for (DWORD i = 0; i < 5; i++) {
        SendMessage(inputBoxes[currentLine - 1][i], EM_SETREADONLY, 1, NULL);
        SendMessage(inputBoxes[currentLine][i], EM_SETREADONLY, 0, NULL);
    }
    SetFocus(inputBoxes[currentLine][0]);
}

void ResetGame(HWND inputBoxes[6][5]) { //set all except first line of textboxes to read only and set their IDs back to 0
    for (DWORD i = 0; i < 6; i++) {
        for (DWORD j = 0; j < 5; j++) {
            SendMessage(inputBoxes[i][j], EM_SETREADONLY, 0, NULL);
            SetWindowText(inputBoxes[i][j], L"");
            SetWindowLongPtr(inputBoxes[i][j], GWLP_ID, 0);

            if (i != 0) {
                SendMessage(inputBoxes[i][j], EM_SETREADONLY, 1, NULL);
            }
        }
    }
}

char** LoadValidWords(FILE* fileOfWords, bool readFromValidAnswersList) { //load list of valid words and answer words
    char** listOfWords = NULL;

    if (!readFromValidAnswersList) { //open file of valid words
        fileOfWords = fopen("valid-wordle-words.txt", "r");
        if (!fileOfWords) {
            MessageBox(NULL, L"Error opening file", NULL, NULL);
            fclose(fileOfWords);
        }

        listOfWords = (char**)malloc(NUMBER_OF_VALID_WORDS * sizeof(char*));

        for (DWORD i = 0; i < NUMBER_OF_VALID_WORDS; i++) {
            listOfWords[i] = (char*)malloc(6 * sizeof(char));
        }
    }

    if (readFromValidAnswersList) {
        fileOfWords = fopen("wordle-targets.txt", "r");
        if (!fileOfWords) {
            MessageBox(NULL, L"Error opening file", NULL, NULL);
            fclose(fileOfWords);
        }

        listOfWords = (char**)malloc(NUMBER_OF_ANSWER_WORDS * sizeof(char*));

        for (DWORD i = 0; i < NUMBER_OF_ANSWER_WORDS; i++) {
            listOfWords[i] = (char*)malloc(6 * sizeof(char));
        }
    }

    char buffer[6] = { 0 };
    DWORD index = 0;

    if (!readFromValidAnswersList) {
        while (fscanf(fileOfWords, "%s", buffer) != EOF || index < NUMBER_OF_VALID_WORDS) {
            buffer[5] = '\0';
            strcpy(listOfWords[index], buffer);
            memset(buffer, 0, sizeof(char));
            index++;
        }
    }

    if (readFromValidAnswersList) {
        while (fscanf(fileOfWords, "%s", buffer) != EOF || index < NUMBER_OF_ANSWER_WORDS) {
            buffer[5] = '\0';
            strcpy(listOfWords[index], buffer);
            memset(buffer, 0, sizeof(char));
            index++;
        }
    }

    return listOfWords;
}

char* GetRandomWord(char** listOfWords) { //get a random word from list of answer words
    srand(time(NULL));
    int randomNumber = rand() % NUMBER_OF_ANSWER_WORDS;
    char* randomWord = listOfWords[randomNumber];

    return randomWord;
}