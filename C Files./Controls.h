#pragma once
#include <windows.h>

HFONT SetHeaderFontAttributes();
HFONT SetHeader2FontAttributes();
HFONT SetFontAttributes();
HFONT SetInputFontAttributes();

void MoveFocus(HWND inputBoxes[6][5], DWORD currentLine);
void Backspace(HWND inputBoxes[6][5], DWORD currentLine);