#include "Controls.h"

HFONT SetHeaderFontAttributes() {
    HFONT hHeaderFont;
    return hHeaderFont = CreateFont(45, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
}

HFONT SetHeader2FontAttributes() {
    HFONT hHeaderFont;
    return hHeaderFont = CreateFont(24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
}

HFONT SetFontAttributes() {
    HFONT hFont;
    return hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
}

HFONT SetInputFontAttributes() {
    HFONT hInputFont;
    return hInputFont = CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
}

void MoveFocus(HWND inputBoxes[6][5], DWORD currentLine) {
    WCHAR charBuffer[2] = { 0 };

    for (DWORD i = 0; i < 5; i++) {
        GetWindowText(inputBoxes[currentLine][i], charBuffer, 2);
        if (charBuffer[0] == NULL) {
            SetFocus(inputBoxes[currentLine][i]);
            return;
        }
    }
}

void Backspace(HWND inputBoxes[6][5], DWORD currentLine) {
    WCHAR charBuffer[2] = { 0 };

    for (DWORD i = 5; i > 0; i--) {
        if (inputBoxes[currentLine][i] == GetFocus()) {
            SetFocus(inputBoxes[currentLine][i - 1]);
            return;
        }
    }
}