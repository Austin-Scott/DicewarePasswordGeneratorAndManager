#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#include "Utils.h"
#include "Application.h"

class PasswordDatabase {
private:
	std::vector<std::string> labels;
	std::vector<std::string> passwords;
	std::vector<std::string> dates;
	std::vector<std::string> notes;
	bool unsavedChanges = false;
public:
	PasswordDatabase();
	PasswordDatabase(std::string decryptedFile);
	std::vector<std::string> getLabels();
	std::vector<std::string> getPasswords();
	std::vector<std::string> getDates();
	std::vector<std::string> getNotes();
	bool hasUnsavedChanges();
	void flagUnsavedChanges();
	void printCensoredEntry(int id);
	void printEntry(int id);
	void addNewPassphrase(std::vector<std::string> &wordlist);
	void editEntry(int id, std::vector<std::string> &wordlist);
};