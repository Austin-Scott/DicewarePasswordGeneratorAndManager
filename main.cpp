#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <time.h>
#include <random>

#include "picosha2.h"

using namespace std;

#include "Utils.h"
#include "PasswordDatabase.h"
#include "Encryption.h"
#include "Application.h"


int main(int argc, char* argv[]) {

	string nameOfWordList = "wordlist.txt";
	string nameOfPasswords = "passwords.txt";

	vector<string> wordlist;
	string encryptedPasswordsData;

	PasswordDatabase db;
	uint32_t key = 0;
	uint32_t salt = 0;
	string passphrase = "";

	if (argc == 1) {
		//use default file names and locations
	}
	else if(argc==3)  {
		nameOfWordList = argv[1];
		nameOfPasswords = argv[2];
	}
	else {
		//illegal number of arguments. Display help and halt.
		cout << "Illegal number of cmd-line arguments. Correct usage:" << endl << endl
			<< argv[0] << endl
			<< "\tStarts the program with default wordlist and password filenames/locations." << endl
			<< "\tThe default files are: \"" + nameOfWordList + "\" and \"" + nameOfPasswords + "\"." << endl
			<< argv[0] << " [Path to wordlist] [Path to passwords]" << endl;
		return 0;
	}

	ifstream wordListFile;
	wordListFile.open(nameOfWordList);
	if (!wordListFile) {
		cout << "Error: enable to open \"" + nameOfWordList + "\". Halting." << endl;
		return 0;
	} 
	while (!wordListFile.eof()) {
		string word="";
		getline(wordListFile, word, '\n');
		wordlist.push_back(word);
	}
	wordListFile.close();

	ifstream passwordsFile;
	passwordsFile.open(nameOfPasswords, ios::binary);

	if (!passwordsFile) {
		cout << "Unable to open passwords file. Entering first time setup." << endl;

		cout << "Creating random salt..." << endl;
		salt = generateNewKey("salt");

		cout << "Done creating random salt. Creating key..." << endl;
		key = generateNewKey("key");

		cout << "Done creating key. Creating passphrase..." << endl;
		passphrase = generatePassphrase(wordlist);
		cout << "Passphrase created." << endl
			<< endl << "-----------------------------" << endl
			<< "KEY: " << setw(8) << setfill('0') << right << hex << key << endl;
		cout << dec;
		cout<< "PASSPHRASE: " << passphrase << endl
			<< "-----------------------------" << endl << endl
			<< "***NOTE: These are required to access your passwords." << endl
			<< "Recovery of any password is not possible if you lose these." << endl
			<< "Type \"yes\" if you acknowledge this notice>";

		flushCin();
		while (true) {
			string input = "";
			getline(cin, input);
			if (input == "yes") break;
			cout << "You must type \"yes\" and agree to the notice to continue using this software." << endl
				<< "Type \"yes\" if you acknowledge this notice: ";
		}
		//We must have at least one entry in the database before we save it now
		//writeAndEncryptFile(db, key, salt, passphrase, nameOfPasswords);
		cout << "Password database created." << endl;
	}
	else {
		passwordsFile.read((char *)&salt, sizeof(uint32_t));

		char rawHash[32];
		passwordsFile.read(rawHash, sizeof(char)*32);
		string hash = "";
		for (int i = 0; i < 32; i++) hash += rawHash[i];

		encryptedPasswordsData.assign((istreambuf_iterator<char>(passwordsFile)),
			(istreambuf_iterator<char>()));

		string decrypted=decryptFile(db, key, passphrase, salt, hash, encryptedPasswordsData);
		passwordsFile.close();
		db = PasswordDatabase(decrypted);
	}
	
	cout << endl;
	string greeting = "Welcome to diceware password generator and manager!\nSoftware by Metgame (Austin Scott) V1.3";
	printBox(greeting);
	cout << endl;

	bool keeplooping = true;
	while (keeplooping) {
		cout << "Menu:\n\t1. Add new passphrase.\n\t2. View existing passphrase.\n\t3. Edit existing passphrases.\n\t4. Edit database key/passphrase.\n\t5. ";
		if (db.getLabels().size() > 0) {
			cout << "Save and ";
		}
		cout << "Quit" << endl;
		
		if (db.getLabels().size() == 0) {
			cout << "***NOTE: You must have at least one passphrase before you may save the database." << endl;
		}
		else if (db.hasUnsavedChanges()) {
			cout << "***WARNING: You have unsaved changes." << endl;
		}
		
		else {
			cout << endl;
		}
		int choice = 0;
		cout << "Choose option>";
		cin >> choice;
		int selection = 0;
		flushCin();
		switch (choice) {
		case 1:
			db.addNewPassphrase(wordlist);
			break;
		case 2:
			selection = searchForEntry(db);
			if (selection == -1) break;
			db.printEntry(selection);
			break;
		case 3:
			selection = searchForEntry(db);
			if (selection == -1) break;
			db.editEntry(selection, wordlist);
			break;
		case 4:
			changeKeyPass(db, salt, key, passphrase, wordlist);
			break;
		case 5:
			if (db.getLabels().size() > 0) {
				writeAndEncryptFile(db, key, salt, passphrase, nameOfPasswords);
			}
			else {
				cout << "Exiting without saving..." << endl;
			}
			keeplooping = false;
			break;
		default:
			cout << "Error: invalid choice. Please choose again." << endl;
		}
	}

	return 0;
}