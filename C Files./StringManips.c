#pragma once
#include "StringManips.h"

WCHAR* AppendWildcard(WCHAR directory[]) { //appends a search wildcard to a WCHAR array
	wcscat(directory, L"\\*");

	return directory;
}

WCHAR* AppendNewFileExtension(WCHAR file[]) { //appends .ware extension to a WCHAR array and returns it
	wcscat(file, L".ware");

	return file;
}

WCHAR* GetFileExtension(WCHAR file[]) {
	//get the length of the file path and move index to the end of it
	DWORD index = wcslen(file);

	//decrement until a '.' is found. If none is found, return NULL
	while (file[index] != '.') {

		if (index == 0) {
			return NULL;
		}
		index--;
	}

	//create a WCHAR array to hold new file path, one without its file extension
	WCHAR fileExtension[MAX_PATH * sizeof(WCHAR)] = { 0 };
	DWORD index2 = 0;

	fileExtension[index2] = file[index];
	index++;
	index2++;

	//copy file extension to new array
	while (file[index] != '\0') {
		fileExtension[index2] = file[index];
		index++;
		index2++;
	}

	return fileExtension;
}

WCHAR* RemoveFileExtension(WCHAR file[]) {
	//get the length of the file path and move index to the end of it
	DWORD index = wcslen(file);

	//decrement until a '.' is found. If none is found, return NULL
	while (file[index] != '.') {
		if (index == 0) {
			return NULL;
		}
		index--;
	}

	//create a WCHAR array to hold new file path, one without its file extension
	WCHAR newFile[MAX_PATH * sizeof(WCHAR)] = { 0 };
	DWORD index2 = 0;

	//copy file path up to its file extension to the new array
	for (index2; index2 < index; index2++) {
		newFile[index2] = file[index2];
	}
	return newFile;
}

void GetTargetDirectory(WCHAR* targetDirectory) {
	WCHAR username[MAX_PATH * sizeof(WCHAR)];
	DWORD length = MAX_PATH * sizeof(WCHAR);

	//get the user's username
	GetUserNameW(username, &length);

	//Form WCHAR array that will hold the directory
	wcscpy(targetDirectory, L"C:\\Users\\");
	wcscat(targetDirectory, username);
	wcscat(targetDirectory, L"\\Users");
}

bool MatchesFileExtension(WCHAR fileExtension[], WCHAR listOfExtensions[]) { //checks if a given file name has a file extension that matches one in a given list
	bool fileExtensionPresent = false;

	//if array is NULL, return false
	if (fileExtension == NULL) {
		return false;
	}

	//loop through entire list of extensions
	for (DWORD index = 0; index < wcslen(listOfExtensions);) {
		if (listOfExtensions[index] == '.') { //if a '.' is found, compare every character after the "." to the file extension passed
			DWORD count = 1;
			index++;
			while (listOfExtensions[index] != '.' && index <= wcslen(listOfExtensions)) { //compare until the next "." is found or the end of the list is reached

				if (fileExtension[count] == listOfExtensions[index]) {
					fileExtensionPresent = true;
					index++;
					count++;
				}

				else {
					fileExtensionPresent = false;
					break;
				}
			}
		}
		if (fileExtensionPresent) {
			break;
		}
		index++;
	}
	return fileExtensionPresent;
}