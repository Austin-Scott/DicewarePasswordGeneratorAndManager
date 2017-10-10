#include "Application.h"

string generatePassphrase(vector<string>& wordlist) {
	string result = "";
	string append = "";
	int words = 0;
	while (true) {
		cout << "How many words should the passphrase be?>";
		cin >> words;
		if (words > 0) break;
		cout << "Error: number of words must be greater than zero. Try again." << endl;
	}
	cout << "Do you wish to append any special characters to the end of your passphrase? (y/n)";
	if (_getch() == 0x79) {
		cout << endl << "Please add any characters you would like to append to your passphrase>" << endl;
		cin >> append;
	}
	cout << endl;

	int selection = 0;
	random_device rd;

	while (true) {
		flushCin();
		cout << "What method would you like to use to generate your passphrase?\n\n"
			<< "\t1. Hardware based true random number generator (if available)\n"
			<< "\t2. Real-world dice\n"
			<< "\t3. Other (manual entry of ranged random numbers)\n"
			<< "\t4. Pseudo-random number generator (not recommended)\n\n";

		cout << "Choose option>";
		cin >> selection;
		if (selection >= 1 && selection <= 4) {
			if (selection == 1) { //Check if TRNG even exists
				if (rd.entropy() > 0) { //TRNG exists on users machine
					cout << "TRNG detected. Continuing..." << endl;
					break;
				}
				else {
					cout << "Unfortunately, your device does not support hardware based random number generation." << endl
						<< "Please choose a different option." << endl;
				}
			}
			else {
				cout << endl;
				break;
			}
		}
		else {
			cout << "Error: invalid choice. Please try again." << endl;
		}
	}

	if (selection == 1) {

		uniform_int_distribution<int> dist(0, wordlist.size() - 1);
		for (int i = 0; i < words; i++) {
			result += wordlist[dist(rd)];
			if (i < words - 1 || !append.empty()) result += " ";
		}

	}
	else if (selection == 2) {

		for (int i = 0; i < words; i++) {
			cout << "Selecting word (" << (i + 1) << "/" << words << ")..." << endl;
			result += wordlist[getRandomNumberFromDie(wordlist.size())];
			if (i < words - 1 || !append.empty()) result += " ";
		}

	}
	else if (selection == 3) {

		for (int i = 0; i < words; i++) {
			flushCin();
			cout << "Selecting word (" << (i + 1) << "/" << words << ")..." << endl;
			int input = 0;
			cout << "Enter a random number inclusively between 1 and " << wordlist.size() << ">";
			cin >> input;
			if (input >= 1 && input <= wordlist.size()) {
				result += wordlist[input - 1];
				if (i < words - 1 || !append.empty()) result += " ";
			}
			else {
				cout << "Error: invalid input. Please try again." << endl;
				i--;
			}
		}

	}
	else {

		//Passphrase generation method: PRNG
		seedGenerator(generateNewKey("seed"));
		for (int i = 0; i < words; i++) {
			result += wordlist[random() % wordlist.size()];
			if (i < words - 1 || !append.empty()) result += " ";
		}

	}


	result += append;

	cout << endl << "Copying generated passphrase to your clipboard..." << endl;
	toClipboard(result);

	cout << endl << "-Statistics for this passphrase-" << endl;
	double totalPossibleWordCombos = pow(wordlist.size(), words);
	cout << "Current wordlist is " << wordlist.size() << " words long." << endl;
	cout << "Based of passphrase length and size of your wordlist total possible passphrases:" << endl << totalPossibleWordCombos << endl;
	cout << "Assuming your adversary is capable of a trillion guesses per second and they know your wordlist, length of passphrase, and any characters you added to the end-\nAverage time needed to crack this passphrase in years:" << endl;
	double guessesPerYear = 1000000000000.0*60.0*60.0*24.0*365.0;
	double maxYearsToGuess = totalPossibleWordCombos / guessesPerYear;
	double averageYearsToGuess = maxYearsToGuess / 2.0;
	cout << "About " << averageYearsToGuess << " years. Heat death of the universe in about 1x10^100 years." << endl;

	cout << endl << "Press enter to continue...";
	while (_getch() != 0x0D);
	cout << endl << endl;
	cout << "Clearing passphrase from your clipboard..." << endl;
	string empty = "";
	toClipboard(empty);
	cout << endl;
	return result;
}

int searchForEntry(PasswordDatabase & db) {
	string input = "";
	int numOfResults = 0;
	vector<int> matchedResultIDs;
	while (true) {
		numOfResults = 0;
		matchedResultIDs.clear();
		cout << "Please enter search term>";
		getline(cin, input);
		for (int i = 0; i < db.getLabels().size(); i++) {
			if (db.getLabels()[i].find(input) != string::npos) {
				numOfResults++;
				matchedResultIDs.push_back(i);
			}
		}
		if (numOfResults > 0) {
			cout << numOfResults << " result/s found:" << endl;
			break;
		}
		else {
			cout << "No results were found. Would you like to try again? (Y/N)";
			getline(cin, input);
			if (input != "Y") return -1;
			cout << endl;
		}
	}
	for (int i = 0; i < matchedResultIDs.size(); i++) {
		cout << "\t" << (i + 1) << ". ";
		db.printCensoredEntry(matchedResultIDs[i]);
	}
	while (true) {
		cout << endl << "Please select an entry>";
		getline(cin, input);
		if (toInt(input) >= 1 && toInt(input) <= matchedResultIDs.size()) {
			return matchedResultIDs[toInt(input) - 1];
		}
		else {
			cout << "Error: Invalid input. Please try again." << endl;
		}
	}
}

uint32_t generateNewKey(string name) {
	uint32_t result = 0;

	int selection = 0;
	random_device rd;

	cout << "New source of entropy required." << endl;
	while (true) {

		cout << "What method would you like to use to create the " << name << "?\n\n"
			<< "\t1. Hardware based true random number generator (if available)\n"
			<< "\t2. Random keyboard input\n"
			<< "\t3. Other (manual entry of a random key in hex)\n"
			<< "\t4. Current system time (not recommended)\n\n";

		cout << "Choose option>";
		cin >> selection;
		if (selection >= 1 && selection <= 4) {
			if (selection == 1) { //Check if TRNG even exists
				if (rd.entropy() > 0) { //TRNG exists on users machine
					cout << "TRNG detected. Continuing..." << endl;
					break;
				}
				else {
					cout << "Unfortunately, your device does not support hardware based random number generation." << endl
						<< "Please choose a different option." << endl;
				}
			}
			else {
				cout << endl;
				break;
			}
		}
		else {
			cout << "Error: invalid choice. Please try again." << endl;
		}
		flushCin();
	}

	if (selection == 1) {

		cout << "Getting key from TRNG..." << endl;
		uniform_int_distribution<uint32_t> dist(0U, 4294967295U);
		result = dist(rd);
		cout << "...Key retrieved." << endl;

	}
	else if (selection == 2) {

		cout << "Please start pressing random characters on your keyboard" << endl;
		for (int i = 0; i < 32; i++) {
			cout << (32 - i) << " random characters remaining...";
			char c = _getch();
			if (c % 2 == 0) {
				result += 0;
			}
			else {
				result += 1;
			}
			result = result << 1;
			cout << endl;
		}
		cout << "Done. Press enter to continue...";
		while (_getch() != 0x0D);
		cout << endl;

	}
	else if (selection == 3) {
		cout << "Please enter a random 8 digit hexadecimal number:" << endl;
		result = getKeyFromUser();
	}
	else {
		cout << "Getting key from current system time..." << endl;
		result = time(NULL);
		cout << "...key retrieved." << endl;
	}

	return result;
}

int getRandomNumberFromDie(int max) { //Number in range [0,max)
	int rollsPerNumber = ceil(log(max) / log(6));
	int largestGeneratableNumber = (int)pow(6, rollsPerNumber);
	int maxUsableNumber = max;
	while (maxUsableNumber + max <= largestGeneratableNumber) maxUsableNumber += max;
	char c = 0;
	int result = 0;
	while (true) {
		result = 0;
		for (int i = 0; i < rollsPerNumber; i++) {
			cout << "Please toss a die and record the result(" << (i + 1) << "/" << rollsPerNumber << ")>";
			c = _getch();
			if (c >= 0x31 && c <= 0x36) {
				switch (c) {
				case 0x31:

					break;
				case 0x32:
					result += 1 * (int)pow(6, i);
					break;
				case 0x33:
					result += 2 * (int)pow(6, i);
					break;
				case 0x34:
					result += 3 * (int)pow(6, i);
					break;
				case 0x35:
					result += 4 * (int)pow(6, i);
					break;
				case 0x36:
					result += 5 * (int)pow(6, i);
					break;
				}
				cout << endl;
			}
			else {
				cout << endl << "Error. Invalid input. Please try again." << endl;
				i--;
			}
		}
		if (result >= maxUsableNumber) {
			cout << "Note: repeating die rolling process for current word. Number created wasn't in usable range." << endl;
		}
		else {
			break;
		}
	}
	return result%max;
}

uint32_t getKeyFromUser() {
	string input;
	bool keepLooping = true;
	while (keepLooping) {
		cout << "Please enter your key>";
		input = "";
		for (int i = 0; i < 8; i++) {
			char c = _getch();

			if (c == 0x8) { //Backspace pressed
				if (input.length() > 0) {
					input.pop_back();
					i--;
					cout << "\b \b"; //Move the cursor back one, overwrite star with space, then move back again
				}
				i--;
			}
			else if (c < 48 || (c > 57 && c < 97) || c>102) {
				//if invalid character has been entered
				cout << endl << "Error: invalid character entered. Please try again." << endl;
				break;
			}
			else {
				cout << "*";
				input += c;
			}
			if (i == 7) keepLooping = false;
		}
	}
	cout << endl;
	uint32_t result = 0;
	for (int i = 7; i >= 0; i--) {
		int n = 0;
		char c = input[i];
		if (c >= 0x30 && c <= 0x39) {
			//this is a number
			n = c - 0x30;
		}
		else {
			//this is a letter
			n = c - 87;
		}
		n *= pow(16, 7 - i);
		result += n;
	}
	return result;
}

string getPassphraseFromUser() {
	string result;
	cout << "Please enter your passphrase>";
	char c = 0;
	while (true) {
		c = _getch();

		if (c == 0xD) break; //Enter pressed
		else if (c == 0x8) { //Backspace pressed
			if (result.length() > 0) {
				result.pop_back();
				cout << "\b \b"; //Move the cursor back one, overwrite star with space, then move back again
			}
		}
		else {
			cout << "*";
			result += c;
		}
	}
	cout << endl;
	return result;
}

string decryptFile(PasswordDatabase & db, uint32_t & key, string & passphrase, uint32_t salt, string hash, string encryptedData) {
	string result = "";
	string saltedResult = "";

	bool stillEncrypted = true;
	while (stillEncrypted) {
		key = getKeyFromUser();
		passphrase = getPassphraseFromUser();
		cout << "Decrypting file..." << endl;
		result = encryptDecrypt(encryptedData, salt, key, passphrase);

		saltedResult = saltString(result, salt);


		if (hash == hashString(saltedResult)) {
			stillEncrypted = false; //success
		}
		else {
			cout << "Error: incorrect key or passphrase. Please try again." << endl;
		}
	}
	cout << "Decryption successful." << endl;
	return result;
}

void writeAndEncryptFile(PasswordDatabase db, uint32_t key, uint32_t salt, string passphrase, string filename) {
	cout << "Saving and encrypting file..." << endl;

	string data = "";

	for (int i = 0; i < db.getLabels().size(); i++) {
		data += db.getLabels()[i] + "\n";
		data += db.getPasswords()[i] + "\n";
		data += db.getDates()[i] + "\n";
		data += db.getNotes()[i];
		if (i != db.getLabels().size() - 1) data += "\n";
	}

	string saltedData = saltString(data, salt);
	string hash = hashString(saltedData);

	data = encryptDecrypt(data, salt, key, passphrase);

	ofstream passwordFile(filename, ofstream::out | ofstream::binary);

	passwordFile.write((char *)&salt, sizeof(uint32_t));
	passwordFile.write(hash.c_str(), sizeof(char) * 32);
	passwordFile.write(data.c_str(), sizeof(char)*data.size());
	passwordFile.close();

	cout << "Done. " << endl;
}

void changeKeyPass(PasswordDatabase & db, uint32_t & salt, uint32_t & key, string & passphrase, vector<string>& wordlist) {
	while (true) {
		cout << "Password database options:\n\t1. Change key.\n\t2. Change passphrase.\n\t3. Change salt.\n\t4. Cancel." << endl << endl;
		int choice = 0;
		cout << "Enter selection>";
		cin >> choice;
		flushCin();
		if (choice == 1) {
			cout << "Changing key..." << endl;
			key = generateNewKey("key");
			cout << "Your new key is: " + toHex(key) << endl;
			cout << "Do not lose this!!" << endl;
			db.flagUnsavedChanges();
			break;
		}
		else if (choice == 2) {
			cout << "Changing passphrase..." << endl;
			passphrase = generatePassphrase(wordlist);
			cout << "Passphrase changed! Do not lose this: \n"
				<< passphrase << endl;
			db.flagUnsavedChanges();
			break;
		}
		else if (choice == 3) {
			cout << "Changing salt..." << endl;
			salt = generateNewKey("salt");
			cout << "Salt changed!" << endl;
			break;

		}
		else if (choice == 4) {
			break;
		}
		else {
			cout << "Error: invalid choice. Please try again." << endl;
		}
	}
}
