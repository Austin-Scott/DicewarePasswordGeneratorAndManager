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

#ifdef _WIN32 //If the current operating system is Windows
	#include <conio.h>
	#include <Windows.h>
	void toClipboard(const std::string &s) { //Modified from: http://www.cplusplus.com/forum/general/48837/
		OpenClipboard(GetDesktopWindow());
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(0x0002, s.size() + 1);
		if (!hg) {
			CloseClipboard();
			std::cout << "Error: Clipboard operation failed." << std::endl;
			return;
		}
		memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(1, hg);
		CloseClipboard();
		GlobalFree(hg);
		if (s.length() > 0) {
			std::cout << "Successfully copied to clipboard." << std::endl;
		}
		else {
			std::cout << "Clipboard contents erased." << std::endl;
		}
	}
#else
	void toClipboard(const std::string &s) {
		std::cout << "This program does not support clipboards on your platform." << std::endl;
	}
	//TODO: _getch() still needs a default replacement based on platform independent code
#endif

using namespace std;

string generatePassphrase(vector<string> &wordlist);
void flushCin();
string getCurrentDateTime();
void printBox(string value);
void editString(string &stringToEdit);
uint32_t getKeyFromUser();

class PasswordDatabase {
private:
	vector<string> labels;
	vector<string> passwords;
	vector<string> dates;
	vector<string> notes;

	bool unsavedChanges = false;

public:
	PasswordDatabase() {
		unsavedChanges = false;
	}
	PasswordDatabase(string decryptedFile) {
		unsavedChanges = false;
		stringstream buffer(decryptedFile);
		string line = "";
		while (!buffer.eof()) {
			getline(buffer, line, '\n');
			labels.push_back(line);
			getline(buffer, line, '\n');
			passwords.push_back(line);
			getline(buffer, line, '\n');
			dates.push_back(line);
			getline(buffer, line, '\n');
			notes.push_back(line);
		}
	}
	vector<string> getLabels() {
		return labels;
	}
	vector<string> getPasswords() {
		return passwords;
	}
	vector<string> getDates() {
		return dates;
	}
	vector<string> getNotes() {
		return notes;
	}

	bool hasUnsavedChanges() {
		return unsavedChanges;
	}

	void flagUnsavedChanges() {
		unsavedChanges = true;
	}

	void printCensoredEntry(int id) {
		/*
		cout << labels[id] << ": " << endl;
		string temp = passwords[id];
		bool afterSpace = true;
		for (int i = 0; i < temp.length(); i++) {
			if (temp[i] != ' ' && !afterSpace) temp[i] = '*';
			else if (temp[i] != ' ') {
				afterSpace = false;
			}
			else {
				temp[i] = '_';
				afterSpace = true;
			}
		}
		cout << "\t" << temp << endl;
		*/

		cout << labels[id] << " (" << dates[id] << ")" << endl;
	}

	void printEntry(int id) {

		string content = "---Password for " + labels[id] + "---\n\n     "
			+ passwords[id]
			+ "     \n\n---Password creation time stamp---\n\n     "
			+ dates[id]
			+ "     \n\n---Notes---\n\n     "
			+ notes[id]
			+ "     \n";
		cout << endl;
		printBox(content);
		cout << endl;

	}

	void addNewPassphrase(vector<string> &wordlist) {
		cout << "What is the name of the site this passphrase is for?>" << endl;
		string name = "";
		getline(cin, name, '\n');
		string passphrase = generatePassphrase(wordlist);

		labels.push_back(name);
		passwords.push_back(passphrase);

		notes.emplace_back("This passphrase has no notes.");

		dates.push_back(getCurrentDateTime());

		unsavedChanges = true;
		cout << "Passphrase added." << endl;
	}

	void editEntry(int id, vector<string> &wordlist) {
		while (true) {
			cout << "Edit options for " + labels[id] + ":\n\t1. Change label.\n\t2. Change passphrase.\n\t3. Delete entry.\n\t4. Edit password notes\n\t5. Cancel edit." << endl << endl;
			int choice = 0;
			cout << "Enter selection> ";
			cin >> choice;
			flushCin();
			if (choice == 1) {
				cout << "Please enter a new name for "+labels[id]+">" << endl;
				editString(labels[id]);
				cout << "Name changed to: " + labels[id] << endl;
				unsavedChanges = true;
				
				break;
			}
			else if (choice == 2) {
				cout << "Changing passphrase for " + labels[id]+"..." << endl;
				passwords[id] = generatePassphrase(wordlist);
				cout << "Passphrase changed." << endl;
				dates[id] = getCurrentDateTime();
				unsavedChanges = true;
				break;
			}
			else if (choice == 3) {
				cout << "Are you sure you want to delete " + labels[id] + "? (yes/no)" << endl;
				
				
				
					string input = "";
					getline(cin, input);
					if (input == "yes") {
						labels[id] = labels.back();
						passwords[id] = passwords.back();
						dates[id] = dates.back();
						notes[id] = notes.back();
						labels.pop_back();
						passwords.pop_back();
						dates.pop_back();
						notes.pop_back();
						cout << "Entry deleted." << endl;
						unsavedChanges = true;
						break;
					}
					
				
			}
			else if (choice == 4) {
				cout << "Editing notes for "<<labels[id]<<">" << endl;
				editString(notes[id]);
				cout << "Notes updated." << endl;
				unsavedChanges = true;
				break;
			}
			else if (choice == 5) {

				break;
			}
			else {
				cout << "Error: invalid choice. Please try again." << endl;
			}
		}
	}
};

string getCurrentDateTime() {
	char buffer[80];
	time_t raw;
	struct tm * timeinfo;
	time(&raw);
	timeinfo = localtime(&raw);
	strftime(buffer, 80, "%B %e %Y %I:%M:%S %p", timeinfo);
	string result = string(buffer);
	return result;
}

int toInt(string value) {
	stringstream buffer;
	buffer << value;
	int result;
	buffer >> result;
	return result;
}

void editString(string &stringToEdit) {
	cout << stringToEdit;
	while (true) {
		char c = _getch();
		if (c == 0xD) { //If enter is pressed stop editing
			cout << endl;
			break;
		}
		else if (c == 0x8) { //If backspace is pressed
			if (stringToEdit.length() > 0) {
				stringToEdit.pop_back();
				cout << "\b \b"; //Move the cursor back one, overwrite star with space, then move back again
			}
		}
		else if (c >= 0x20 && c <= 0x7E) { //If printable character is typed add to end of string
			stringToEdit += c;
			cout << c;
		}
	}
}


int searchForEntry(PasswordDatabase &db) {
	string input = "";
	int numOfResults = 0;
	vector<int> matchedResultIDs;
	while (true) {
		numOfResults = 0;
		matchedResultIDs.clear();
		cout << "Please enter search term>";
		getline(cin, input);
		for (int i = 0; i < db.getLabels().size(); i++) {
			if (db.getLabels()[i].find(input) != string::npos ) {
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

void printBox(string value) {
	const char topLeft = 201;
	const char topRight = 187;
	const char bottomLeft = 200;
	const char bottomRight = 188;
	const char topBottom = 205;
	const char leftRight = 186;

	stringstream buffer;
	buffer << value;
	vector<string> lines;
	while (!buffer.eof()) {
		string line = "";
		getline(buffer, line);
		lines.push_back(line);
	}

	int maxLength = 0;
	for (string s : lines) {
		if (s.length() > maxLength) maxLength = s.length();
	}

	string floor = "";
	floor.assign(maxLength, topBottom);

	string top = "";
	top.push_back(topLeft);
	top += floor;
	top.push_back(topRight);

	cout << top << endl;

	for (string s : lines) {
		cout << leftRight << setw(maxLength) << setfill(' ') << left << s << leftRight << endl;
	}

	string bottom = "";
	bottom.push_back(bottomLeft);
	bottom += floor;
	bottom.push_back(bottomRight);

	cout << bottom << endl;

}

//Random num generator declared here to make sure it is always the same regardless of platform or toolset.
static uint32_t nextSeed = 1;
uint32_t random(void) { //RANDOM_MAX is 32767
	nextSeed = nextSeed * 1103515245 + 12345;
	return (uint32_t)(nextSeed / 65536) % 32768;
}
void seedGenerator(uint32_t seed) {
	nextSeed = seed;
}

string encryptDecrypt(string data, uint32_t key, string passphrase) {
	string result = data;
	seedGenerator(key);
	if (passphrase.length() != 0) {
		for (int i = 0; i < data.length(); i++) {
			result[i] ^= passphrase[random() % passphrase.length()];
		}
	}
	return result;
}

void flushCin() {
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

uint32_t generateNewKey() {
	uint32_t result = 0;

	int selection = 0;
	random_device rd;

	cout << "New source of entropy required." << endl;
	while (true) {
		
		cout << "What method would you like to use to create the key?\n\n"
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

string generatePassphrase(vector<string> &wordlist) {
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
				result += wordlist[input-1];
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
		seedGenerator(generateNewKey());
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

uint32_t getKeyFromUser() {
	string input;
	bool keepLooping = true;
	while (keepLooping) {
		cout << "Please enter your key>";
		input = "";
		for (int i = 0; i < 8;i++) {
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
	for (int i = 7; i>=0; i--) {
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
		else  {
			cout << "*";
			result += c;
		}
	}
	cout << endl;
	return result;
}

string toHex(uint32_t num) {
	stringstream buffer;
	buffer << hex << num;
	string result(buffer.str());
	cout << dec;
	return result;
}

string decryptFile(PasswordDatabase &db, uint32_t &key, string &passphrase, uint32_t checksum, string encryptedData) {
	string result = "";
	uint32_t computedsum = 0;
	while (true) {
		key = getKeyFromUser();
		passphrase = getPassphraseFromUser();
		cout << "Decrypting file..." << endl;
		result=encryptDecrypt(encryptedData, key, passphrase);

		computedsum = 0;
		for (int i = 0; i < result.length(); i++) {
			computedsum += result[i];
		}

		if (computedsum==checksum) {
			break; //success
		}


		cout << "Error: incorrect key or passphrase. Please try again." << endl;
	}
	cout << "Decryption successful." << endl;
	return result;
}

void writeAndEncryptFile(PasswordDatabase db, uint32_t key, string passphrase, string filename) {
	cout << "Saving and encrypting file..." << endl;

	string data = "";

	for (int i = 0; i < db.getLabels().size(); i++) {
		data += db.getLabels()[i] + "\n";
		data += db.getPasswords()[i] + "\n";
		data += db.getDates()[i]+"\n";
		data += db.getNotes()[i];
		if (i != db.getLabels().size() - 1) data += "\n";
	}

	uint32_t checksum = 0;
	for (int i = 0; i < data.length(); i++) {
		checksum += data[i];
	}

	data = encryptDecrypt(data, key, passphrase);

	ofstream passwordFile(filename, ofstream::out | ofstream::binary);
	
	passwordFile.write((char*)&checksum, sizeof(uint32_t));
	passwordFile.write(data.c_str(), sizeof(char)*data.size());
	passwordFile.close();

	cout << "Done. " << endl;
}

void changeKeyPass(PasswordDatabase &db, uint32_t &key, string &passphrase, vector<string> &wordlist) {
	while (true) {
		cout << "Password database options:\n\t1. Change key.\n\t2. Change passphrase.\n\t3. Cancel." << endl << endl;
		int choice = 0;
		cout << "Enter selection>";
		cin >> choice;
		flushCin();
		if (choice == 1) {
			cout << "Changing key..." << endl;
			key = generateNewKey();
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
			break;

		}
		else {
			cout << "Error: invalid choice. Please try again." << endl;
		}
	}
}

int main(int argc, char* argv[]) {

	string nameOfWordList = "wordlist.txt";
	string nameOfPasswords = "passwords.txt";

	vector<string> wordlist;
	string encryptedPasswordsData;

	PasswordDatabase db;
	uint32_t key = 0;
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
		cout << "Creating key..." << endl;
		key = generateNewKey();
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
		writeAndEncryptFile(db, key, passphrase, nameOfPasswords);
		cout << "Password database created." << endl;
	}
	else {
		uint32_t checksum = 0;
		passwordsFile.read((char *)&checksum, sizeof(uint32_t));

		encryptedPasswordsData.assign((istreambuf_iterator<char>(passwordsFile)),
			(istreambuf_iterator<char>()));

		string decrypted=decryptFile(db, key, passphrase, checksum, encryptedPasswordsData);
		passwordsFile.close();
		db = PasswordDatabase(decrypted);
	}
	
	cout << endl;
	string greeting = "Welcome to diceware password generator and manager!\nSoftware by Metgame (Austin Scott) V1.2";
	printBox(greeting);
	cout << endl;

	bool keeplooping = true;
	while (keeplooping) {
		cout << "Menu:\n\t1. Add new passphrase.\n\t2. View existing passphrase.\n\t3. Edit existing passphrases.\n\t4. Edit database key/passphrase.\n\t5. Save and Quit" << endl;
		if (db.hasUnsavedChanges()) {
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
			changeKeyPass(db, key, passphrase, wordlist);
			break;
		case 5:
			writeAndEncryptFile(db, key, passphrase, nameOfPasswords);
			keeplooping = false;
			break;
		default:
			cout << "Error: invalid choice. Please choose again." << endl;
		}
	}

	return 0;
}