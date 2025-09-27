# WORDLEWARE
DON'T RUN THIS IN AN UNPROTECTED ENVIRONMENT. 

THIS PROGRAM DOES NOT STORE THE KEY - IF IT CRASHES OR YOU CLOSE IT WITHOUT WINNING, DATA WILL BE LOST.

Windows Ransomware where the ransom is a game of Wordle. Written in C using WINAPI.

Targeted extensions include:
.jpg, .txt, .ppt, .zip, .7z, .pdf, .docx, .PNG, .png, .log, .xlsx, .flac, .mp4, .mp3, .c, .gif, .doc, .pptx, .wav, .js, .avi, .cpp, .dat, .raw, .json

When WORDLEWARE is run, it will encrypt as many files as it can in the user's directory using AES 256 Encryption.
If the user does not correctly guess the answer in 6 guesses, the game is reset and a new word is chosen. The program will only decrypt the files if a game is won.

List of valid words retrieved from: https://gist.github.com/dracos/dd0668f281e685bad51479e5acaadb93

List of possible answers retrieved from: https://gist.github.com/cfreshman/a03ef2cba789d8cf00c08f767e0fad7b

Heavily inspired by Rensenware: https://github.com/0x00000FF/rensenware-cut
