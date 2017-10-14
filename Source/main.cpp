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

	//Stores the name of the wordlist text file
	string nameOfWordList = "wordlist.txt";

	//Stores the name of the file where passwords are saved to
	string nameOfPasswords = "passwords.txt";

	//Stores the wordlist used for generating new diceware passphrases
	vector<string> wordlist;

	//Object for storing and managing all password data
	PasswordDatabase db;

	//Both of these are used with the encryption functions to prevent adversaries from precomputing solutions
	unsigned char nonce[crypto_aead_xchacha20poly1305_ietf_NPUBBYTES];
	uint32_t salt = 0;

	//Stores the passphrase that the user used to decrypt their passwords file
	string passphrase = "";

	if (!parseCmdArguments(argc, argv, nameOfWordList, nameOfPasswords)) return 0;

	if (!loadWordlist(nameOfWordList, wordlist)) return 1;

	ifstream passwordsFile;
	passwordsFile.open(nameOfPasswords, ios::binary);

	if (!passwordsFile) { //If the passwords file wasn't found perform first time setup

		//Set the nonce and the salt to random values
		randombytes_buf(nonce, sizeof(nonce));
		randombytes_buf(&salt, sizeof(uint32_t));

		firstTimeSetup(passphrase, wordlist);
	}
	else { //Passwords file was found and opened. Read and decrypt its contents.

		//Read the salt and nonce values from the beginning of the file
		passwordsFile.read((char *)&salt, sizeof(uint32_t));
		passwordsFile.read((char *)nonce, sizeof(char)*crypto_aead_xchacha20poly1305_ietf_NPUBBYTES);

		//Everything remaining in the file is cyphertext. Read it all into encryptedPasswordsData.
		string encryptedPasswordsData;
		encryptedPasswordsData.assign((istreambuf_iterator<char>(passwordsFile)),
			(istreambuf_iterator<char>()));

		passwordsFile.close();
		
		//decryptFile() will only return a string once the user has entered the correct passphrase and the cyphertext has been decoded
		string decrypted = decryptFile(db, passphrase, salt, nonce, encryptedPasswordsData);
		
		//Set the nonce and the salt to random values
		randombytes_buf(nonce, sizeof(nonce));
		randombytes_buf(&salt, sizeof(uint32_t));

		//Construct the password database from the information stored in decrypted
		db = PasswordDatabase(decrypted);
	}
	
	cout << endl;
	string greeting = "Welcome to diceware password generator and manager!\nSoftware by Metgame (Austin Scott) V1.4";
	printBox(greeting);
	cout << endl;

	bool keeplooping = true;
	while (keeplooping) {
		cout << "Menu:\n\t1. Add new passphrase.\n\t2. View existing passphrase.\n\t3. Edit existing passphrases.\n\t4. Edit database passphrase.\n\t5. ";

		//The password database has entries and hence can be saved. Label this menu option "Save and Quit" instead of just "Quit"
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

		//Selection is used whenever a choosen option requires performing an action to a specific entry in the database. Selection then stores the corresponding id.
		int selection = 0;

		flushCin();

		switch (choice) {
		case 1:
			db.addNewPassphrase(wordlist);
			break;

		case 2:
			selection = searchForEntry(db);

			//If user canceled the search for an item break
			if (selection == -1) break;

			db.printEntry(selection);
			break;

		case 3:
			selection = searchForEntry(db);

			//If user canceled the search for an item break
			if (selection == -1) break;

			db.editEntry(selection, wordlist);
			break;

		case 4:
			changePassphrase(db, salt, passphrase, wordlist);
			break;

		case 5:
			//Do we have enough entries to save the database?
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