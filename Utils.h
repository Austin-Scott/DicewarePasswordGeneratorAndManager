#pragma once
#define _CRT_SECURE_NO_WARNINGS
/*
Utils.h - A header file for utility functions that don't belong anywhere else
*/

#include <iostream>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <vector>

#ifdef _WIN32 //If the current operating system is Windows
	#include <conio.h>
	#include <Windows.h>
#else
	//TODO: _getch() still needs a default replacement based on platform independent code
#endif

void toClipboard(const std::string &s);

int toInt(std::string value);
void editString(std::string &stringToEdit);
void printBox(std::string value);

//Random num generator declared here to make sure it is always the same regardless of platform or toolset.
static uint32_t nextSeed = 1;
uint32_t random(void);
void seedGenerator(uint32_t seed);

bool getBit(char from, int index);
void xorSetBit(char &to, bool value, int index);
char getByte(uint32_t from, int index);

void flushCin();

std::string toHex(uint32_t num);

std::string getCurrentDateTime();
