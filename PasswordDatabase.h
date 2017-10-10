#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#include "Utils.h"
#include "Application.h"

using namespace std;

class PasswordDatabase {
private:
	vector<std::string> labels;
	vector<std::string> passwords;
	vector<std::string> dates;
	vector<std::string> notes;
	bool unsavedChanges = false;
public:
	PasswordDatabase();
	PasswordDatabase(std::string decryptedFile);
	vector<std::string> getLabels();
	vector<std::string> getPasswords();
	vector<std::string> getDates();
	vector<std::string> getNotes();
	bool hasUnsavedChanges();
	void flagUnsavedChanges();
	void printCensoredEntry(int id);
	void printEntry(int id);
	void addNewPassphrase(vector<std::string> &wordlist);
	void editEntry(int id, vector<std::string> &wordlist);
};