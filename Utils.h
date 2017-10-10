#pragma once
/*
Utils.h - A header file for utility functions that don't belong anywhere else
*/

#include <iostream>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

#ifdef _WIN32 //If the current operating system is Windows
	#include <conio.h>
	#include <Windows.h>
	void toClipboard(const string &s) { //Modified from: http://www.cplusplus.com/forum/general/48837/
		OpenClipboard(GetDesktopWindow());
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(0x0002, s.size() + 1);
		if (!hg) {
			CloseClipboard();
			cout << "Error: Clipboard operation failed." << endl;
			return;
		}
		memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(1, hg);
		CloseClipboard();
		GlobalFree(hg);
		if (s.length() > 0) {
			cout << "Successfully copied to clipboard." << endl;
		}
		else {
			cout << "Clipboard contents erased." << endl;
		}
	}
#else
	void toClipboard(const string &s) {
		cout << "This program does not support clipboards on your platform." << endl;
	}
	//TODO: _getch() still needs a default replacement based on platform independent code
#endif


int toInt(string value);
void editString(string &stringToEdit);
void printBox(string value);

//Random num generator declared here to make sure it is always the same regardless of platform or toolset.
static uint32_t nextSeed = 1;
uint32_t random(void);
void seedGenerator(uint32_t seed);

bool getBit(char from, int index);
void xorSetBit(char &to, bool value, int index);
char getByte(uint32_t from, int index);

void flushCin();

string toHex(uint32_t num);

