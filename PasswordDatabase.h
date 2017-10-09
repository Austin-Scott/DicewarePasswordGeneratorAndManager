#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

string generatePassphrase(vector<string> &wordlist);
void flushCin();
string getCurrentDateTime();
void printBox(string value);
void editString(string &stringToEdit);
uint32_t getKeyFromUser();
string saltString(string str, uint32_t salt);
char getByte(uint32_t from, int index);
string hashString(string str);

class PasswordDatabase {
private:
	vector<string> labels;
	vector<string> passwords;
	vector<string> dates;
	vector<string> notes;
	bool unsavedChanges = false;
public:
	PasswordDatabase();
	PasswordDatabase(string decryptedFile);
	vector<string> getLabels();
	vector<string> getPasswords();
	vector<string> getDates();
	vector<string> getNotes();
	bool hasUnsavedChanges();
	void flagUnsavedChanges();
	void printCensoredEntry(int id);
	void printEntry(int id);
	void addNewPassphrase(vector<string> &wordlist);
	void editEntry(int id, vector<string> &wordlist);
};