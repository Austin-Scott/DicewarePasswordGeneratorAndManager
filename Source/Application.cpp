#include "Application.h"

std::string generatePassphrase(std::vector<std::string>& wordlist) {
	std::string result = "";
	std::string append = "";
	int words = 0;
	while (true) {
		std::cout << "How many words should the passphrase be?>";
		std::cin >> words;
		if (words > 0) break;
		std::cout << "Error: number of words must be greater than zero. Try again." << std::endl;
	}
	std::cout << "Do you wish to append any special characters to the end of your passphrase? (y/n)";
	if (_getch() == 0x79) {
		std::cout << std::endl << "Please add any characters you would like to append to your passphrase>" << std::endl;
		std::cin >> append;
	}
	std::cout << std::endl;

	int selection = 0;
	std::random_device rd;

	while (true) {
		flushCin();
		std::cout << "What method would you like to use to generate your passphrase?\n\n"
			<< "\t1. Hardware based true random number generator (if available)\n"
			<< "\t2. Real-world dice\n"
			<< "\t3. Other (manual entry of ranged random numbers)\n"
			<< "\t4. Pseudo-random number generator (not recommended)\n\n";

		std::cout << "Choose option>";
		std::cin >> selection;
		if (selection >= 1 && selection <= 4) {
			if (selection == 1) { //Check if TRNG even exists
				if (rd.entropy() > 0) { //TRNG exists on users machine
					std::cout << "TRNG detected. Continuing..." << std::endl;
					break;
				}
				else {
					std::cout << "Unfortunately, your device does not support hardware based random number generation." << std::endl
						<< "Please choose a different option." << std::endl;
				}
			}
			else {
				std::cout << std::endl;
				break;
			}
		}
		else {
			std::cout << "Error: invalid choice. Please try again." << std::endl;
		}
	}

	if (selection == 1) {

		std::uniform_int_distribution<int> dist(0, wordlist.size() - 1);
		for (int i = 0; i < words; i++) {
			result += wordlist[dist(rd)];
			if (i < words - 1 || !append.empty()) result += " ";
		}

	}
	else if (selection == 2) {

		for (int i = 0; i < words; i++) {
			std::cout << "Selecting word (" << (i + 1) << "/" << words << ")..." << std::endl;
			result += wordlist[getRandomNumberFromDie(wordlist.size())];
			if (i < words - 1 || !append.empty()) result += " ";
		}

	}
	else if (selection == 3) {

		for (int i = 0; i < words; i++) {
			flushCin();
			std::cout << "Selecting word (" << (i + 1) << "/" << words << ")..." << std::endl;
			int input = 0;
			std::cout << "Enter a random number inclusively between 1 and " << wordlist.size() << ">";
			std::cin >> input;
			if (input >= 1 && input <= wordlist.size()) {
				result += wordlist[input - 1];
				if (i < words - 1 || !append.empty()) result += " ";
			}
			else {
				std::cout << "Error: invalid input. Please try again." << std::endl;
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

	std::cout << std::endl << "Copying generated passphrase to your clipboard..." << std::endl;
	toClipboard(result);

	std::cout << std::endl << "-Statistics for this passphrase-" << std::endl;
	double totalPossibleWordCombos = pow(wordlist.size(), words);
	std::cout << "Current wordlist is " << wordlist.size() << " words long." << std::endl;
	std::cout << "Based of passphrase length and size of your wordlist total possible passphrases:" << std::endl << totalPossibleWordCombos << std::endl;
	std::cout << "Assuming your adversary is capable of a trillion guesses per second and they know your wordlist, length of passphrase, and any characters you added to the end-\nAverage time needed to crack this passphrase in years:" << std::endl;
	double guessesPerYear = 1000000000000.0*60.0*60.0*24.0*365.0;
	double maxYearsToGuess = totalPossibleWordCombos / guessesPerYear;
	double averageYearsToGuess = maxYearsToGuess / 2.0;
	std::cout << "About " << averageYearsToGuess << " years. Heat death of the universe in about 1x10^100 years." << std::endl;

	std::cout << std::endl << "Press enter to continue...";
	while (_getch() != 0x0D);
	std::cout << std::endl << std::endl;
	std::cout << "Clearing passphrase from your clipboard..." << std::endl;
	std::string empty = "";
	toClipboard(empty);
	std::cout << std::endl;
	return result;
}

int searchForEntry(PasswordDatabase &db) {
	std::string input = "";
	int numOfResults = 0;
	std::vector<int> matchedResultIDs;
	while (true) {
		numOfResults = 0;
		matchedResultIDs.clear();
		std::cout << "Please enter search term>";
		getline(std::cin, input);
		for (int i = 0; i < db.getLabels().size(); i++) {
			if (db.getLabels()[i].find(input) != std::string::npos) {
				numOfResults++;
				matchedResultIDs.push_back(i);
			}
		}
		if (numOfResults > 0) {
			std::cout << numOfResults << " result/s found:" << std::endl;
			break;
		}
		else {
			std::cout << "No results were found. Would you like to try again? (Y/N)";
			getline(std::cin, input);
			if (input != "Y") return -1;
			std::cout << std::endl;
		}
	}
	for (int i = 0; i < matchedResultIDs.size(); i++) {
		std::cout << "\t" << (i + 1) << ". ";
		db.printCensoredEntry(matchedResultIDs[i]);
	}
	while (true) {
		std::cout << std::endl << "Please select an entry>";
		getline(std::cin, input);
		if (toInt(input) >= 1 && toInt(input) <= matchedResultIDs.size()) {
			return matchedResultIDs[toInt(input) - 1];
		}
		else {
			std::cout << "Error: Invalid input. Please try again." << std::endl;
		}
	}
}

uint32_t generateNewKey(std::string name) {
	uint32_t result = 0;

	int selection = 0;
	std::random_device rd;

	std::cout << "New source of entropy required." << std::endl;
	while (true) {

		std::cout << "What method would you like to use to create the " << name << "?\n\n"
			<< "\t1. Hardware based true random number generator (if available)\n"
			<< "\t2. Random keyboard input\n"
			<< "\t3. Other (manual entry of a random key in hex)\n"
			<< "\t4. Current system time (not recommended)\n\n";

		std::cout << "Choose option>";
		std::cin >> selection;
		if (selection >= 1 && selection <= 4) {
			if (selection == 1) { //Check if TRNG even exists
				if (rd.entropy() > 0) { //TRNG exists on users machine
					std::cout << "TRNG detected. Continuing..." << std::endl;
					break;
				}
				else {
					std::cout << "Unfortunately, your device does not support hardware based random number generation." << std::endl
						<< "Please choose a different option." << std::endl;
				}
			}
			else {
				std::cout << std::endl;
				break;
			}
		}
		else {
			std::cout << "Error: invalid choice. Please try again." << std::endl;
		}
		flushCin();
	}

	if (selection == 1) {

		std::cout << "Getting key from TRNG..." << std::endl;
		std::uniform_int_distribution<uint32_t> dist(0U, 4294967295U);
		result = dist(rd);
		std::cout << "...Key retrieved." << std::endl;

	}
	else if (selection == 2) {

		std::cout << "Please start pressing random characters on your keyboard" << std::endl;
		for (int i = 0; i < 32; i++) {
			std::cout << (32 - i) << " random characters remaining...";
			char c = _getch();
			if (c % 2 == 0) {
				result += 0;
			}
			else {
				result += 1;
			}
			result = result << 1;
			std::cout << std::endl;
		}
		std::cout << "Done. Press enter to continue...";
		while (_getch() != 0x0D);
		std::cout << std::endl;

	}
	else if (selection == 3) {
		std::cout << "Please enter a random 8 digit hexadecimal number:" << std::endl;
		result = getKeyFromUser();
	}
	else {
		std::cout << "Getting key from current system time..." << std::endl;
		result = time(NULL);
		std::cout << "...key retrieved." << std::endl;
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
			std::cout << "Please toss a die and record the result(" << (i + 1) << "/" << rollsPerNumber << ")>";
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
				std::cout << std::endl;
			}
			else {
				std::cout << std::endl << "Error. Invalid input. Please try again." << std::endl;
				i--;
			}
		}
		if (result >= maxUsableNumber) {
			std::cout << "Note: repeating die rolling process for current word. Number created wasn't in usable range." << std::endl;
		}
		else {
			break;
		}
	}
	return result%max;
}

uint32_t getKeyFromUser() {
	std::string input;
	bool keepLooping = true;
	while (keepLooping) {
		std::cout << "Please enter your key>";
		input = "";
		for (int i = 0; i < 8; i++) {
			char c = _getch();

			if (c == 0x8) { //Backspace pressed
				if (input.length() > 0) {
					input.pop_back();
					i--;
					std::cout << "\b \b"; //Move the cursor back one, overwrite star with space, then move back again
				}
				i--;
			}
			else if (c < 48 || (c > 57 && c < 97) || c>102) {
				//if invalid character has been entered
				std::cout << std::endl << "Error: invalid character entered. Please try again." << std::endl;
				break;
			}
			else {
				std::cout << "*";
				input += c;
			}
			if (i == 7) keepLooping = false;
		}
	}
	std::cout << std::endl;
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

std::string getPassphraseFromUser() {
	std::string result;
	std::cout << "Please enter your passphrase>";
	char c = 0;
	while (true) {
		c = _getch();

		if (c == 0xD) break; //Enter pressed
		else if (c == 0x8) { //Backspace pressed
			if (result.length() > 0) {
				result.pop_back();
				std::cout << "\b \b"; //Move the cursor back one, overwrite star with space, then move back again
			}
		}
		else {
			std::cout << "*";
			result += c;
		}
	}
	std::cout << std::endl;
	return result;
}

std::string decryptFile(PasswordDatabase & db, uint32_t & key, std::string & passphrase, uint32_t salt, std::string hash, std::string encryptedData) {
	std::string result = "";
	std::string saltedResult = "";

	bool stillEncrypted = true;
	while (stillEncrypted) {
		key = getKeyFromUser();
		passphrase = getPassphraseFromUser();
		std::cout << "Decrypting file..." << std::endl;
		result = encryptDecrypt(encryptedData, salt, key, passphrase);

		saltedResult = saltString(result, salt);


		if (hash == hashString(saltedResult)) {
			stillEncrypted = false; //success
		}
		else {
			std::cout << "Error: incorrect key or passphrase. Please try again." << std::endl;
		}
	}
	std::cout << "Decryption successful." << std::endl;
	return result;
}

void writeAndEncryptFile(PasswordDatabase db, uint32_t key, uint32_t salt, std::string passphrase, std::string filename) {
	std::cout << "Saving and encrypting file..." << std::endl;

	std::string data = "";

	for (int i = 0; i < db.getLabels().size(); i++) {
		data += db.getLabels()[i] + "\n";
		data += db.getPasswords()[i] + "\n";
		data += db.getDates()[i] + "\n";
		data += db.getNotes()[i];
		if (i != db.getLabels().size() - 1) data += "\n";
	}

	std::string saltedData = saltString(data, salt);
	std::string hash = hashString(saltedData);

	data = encryptDecrypt(data, salt, key, passphrase);

	ofstream passwordFile(filename, ofstream::out | ofstream::binary);

	passwordFile.write((char *)&salt, sizeof(uint32_t));
	passwordFile.write(hash.c_str(), sizeof(char) * 32);
	passwordFile.write(data.c_str(), sizeof(char)*data.size());
	passwordFile.close();

	std::cout << "Done. " << std::endl;
}

void changeKeyPass(PasswordDatabase & db, uint32_t & salt, uint32_t & key, std::string & passphrase, std::vector<std::string>& wordlist) {
	while (true) {
		std::cout << "Password database options:\n\t1. Change key.\n\t2. Change passphrase.\n\t3. Change salt.\n\t4. Cancel." << std::endl << std::endl;
		int choice = 0;
		std::cout << "Enter selection>";
		std::cin >> choice;
		flushCin();
		if (choice == 1) {
			std::cout << "Changing key..." << std::endl;
			key = generateNewKey("key");
			std::cout << "Your new key is: " + toHex(key) << std::endl;
			std::cout << "Do not lose this!!" << std::endl;
			db.flagUnsavedChanges();
			break;
		}
		else if (choice == 2) {
			std::cout << "Changing passphrase..." << std::endl;
			passphrase = generatePassphrase(wordlist);
			std::cout << "Passphrase changed! Do not lose this: \n"
				<< passphrase << std::endl;
			db.flagUnsavedChanges();
			break;
		}
		else if (choice == 3) {
			std::cout << "Changing salt..." << std::endl;
			salt = generateNewKey("salt");
			std::cout << "Salt changed!" << std::endl;
			break;

		}
		else if (choice == 4) {
			break;
		}
		else {
			std::cout << "Error: invalid choice. Please try again." << std::endl;
		}
	}
}
