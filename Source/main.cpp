#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

#define SODIUM_STATIC 1
#define SODIUM_EXPORT
#include <sodium.h>

#include "PasswordDatabase.h"
#include "Utils.h"
#include "Encryption.h"
#include "Application.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (sodium_init() < 0) {
		cout << "Error: Unable to initialize libsodium. Halting." << endl;
		return 2;
	}

	string nameOfWordList = "wordlist.txt";
	string nameOfPasswords = "passwords.txt";

	vector<string> wordlist;
	string encryptedPasswordsData;

	PasswordDatabase db;
	unsigned char nonce[crypto_aead_xchacha20poly1305_ietf_NPUBBYTES];
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
		return 1;
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

		randombytes_buf(nonce, sizeof(nonce));

		randombytes_buf(&salt, sizeof(uint32_t));

		cout << "Creating passphrase..." << endl;
		passphrase = generatePassphrase(wordlist);
		cout << "Passphrase created." << endl
			<< endl << "-----------------------------" << endl;
		cout<< "PASSPHRASE: " << passphrase << endl
			<< "-----------------------------" << endl << endl
			<< "***NOTE: This is required to access your passwords." << endl
			<< "Recovery of any password is not possible if you lose this." << endl
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

		passwordsFile.read((char *)nonce, sizeof(char)*crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

		encryptedPasswordsData.assign((istreambuf_iterator<char>(passwordsFile)),
			(istreambuf_iterator<char>()));

		string decrypted = decryptFile(db, passphrase, salt, nonce, encryptedPasswordsData);
		passwordsFile.close();

		randombytes_buf(nonce, sizeof(nonce));
		randombytes_buf(&salt, sizeof(uint32_t));

		db = PasswordDatabase(decrypted);
	}
	
	cout << endl;
	string greeting = "Welcome to diceware password generator and manager!\nSoftware by Metgame (Austin Scott) V1.4";
	printBox(greeting);
	cout << endl;

	bool keeplooping = true;
	while (keeplooping) {
		cout << "Menu:\n\t1. Add new passphrase.\n\t2. View existing passphrase.\n\t3. Edit existing passphrases.\n\t4. Edit database passphrase.\n\t5. ";
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
			changeKeyPass(db, salt, passphrase, wordlist);
			break;
		case 5:
			if (db.getLabels().size() > 0) {
				writeAndEncryptFile(db, salt, nonce, passphrase, nameOfPasswords);
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