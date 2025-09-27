#pragma once
#include <windows.h>
#include <stdbool.h>

WCHAR* AppendWildcard(WCHAR directory[]);
WCHAR* AppendNewFileExtension(WCHAR file[]);
WCHAR* GetFileExtension(WCHAR file[]);
WCHAR* RemoveFileExtension(WCHAR file[]);
void GetTargetDirectory(WCHAR* targetDirectory);
bool MatchesFileExtension(WCHAR fileExtension[], WCHAR listOfExtensions[]);