#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <conio.h>

using namespace std;

string generatePassphrase(vector<string> &wordlist);

class PasswordDatabase {
private:
	vector<string> labels;
	vector<string> passwords;

public:
	PasswordDatabase() {

	}
	PasswordDatabase(string decryptedFile) {
		stringstream buffer;
		string line = "";
		getline(buffer, line, '\n');
		while (!buffer.eof()) {
			getline(buffer, line, '\n');
			labels.push_back(line);
			getline(buffer, line, '\n');
			passwords.push_back(line);
		}
	}
	vector<string> getLabels() {
		return labels;
	}
	vector<string> getPasswords() {
		return passwords;
	}

	void printCensoredEntry(int id) {
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
	}

	void printEntry(int id) {
		cout << labels[id] << ": " << endl;
		cout << "\t" << passwords[id] << endl;
	}

	void addNewPassphrase(vector<string> &wordlist) {
		cout << "What is the name of the site this passphrase is for?" << endl;
		string name = "";
		getline(cin, name, '\n');
		string passphrase = generatePassphrase(wordlist);

		labels.push_back(name);
		passwords.push_back(passphrase);

		cout << "Passphrase added." << endl;
	}
};

int toInt(string value) {
	stringstream buffer;
	buffer << value;
	int result;
	buffer >> result;
	return result;
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
		cout << (i + 1) << ". ";
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
	for (int i = 0; i < data.length(); i++) {
		result[i] ^= passphrase[random() % passphrase.length()];
	}
	return result;
}

void flushCin() {
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

uint32_t generateNewKey() {
	uint32_t result = 0;
	cout << "New source of entropy required." << endl;
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
	return result;
}

string generatePassphrase(vector<string> &wordlist) {
	string result = "";
	string append = "";
	int words = 0;
	while (true) {
		cout << "How many words should the passphrase be?";
		cin >> words;
		if (words > 0) break;
		cout << "Error: number of words must be greater than zero. Try again." << endl;
	}
	cout << "Do you wish to append any special characters to the end of your passphrase? (y/n)";
	if (_getch() == 0x79) {
		cout << endl << "Please add any characters you would like to append to your passphrase: " << endl;
		cin >> append;
	}
	cout << endl;
	seedGenerator(generateNewKey());
	for (int i = 0; i < words; i++) {
		result += wordlist[random() % wordlist.size()];
		if (i < words - 1 || !append.empty()) result += " ";
	}
	result += append;
	cout << "Done. Your new passphrase is:" << endl;
	cout << result << endl;
	cout << "Press enter to continue...";
	while (_getch() != 0x0D);
	cout << endl;
	return result;
}

uint32_t getKeyFromUser() {
	string input;
	bool keepLooping = true;
	while (keepLooping) {
		cout << "Please enter your key:";
		input = "";
		for (int i = 0; i < 8;i++) {
			char c = _getch();
			if (c < 48 || (c > 57 && c < 97) || c>102) {
				//if invalid character has been entered
				cout << "Error: invalid character entered. Please try again." << endl;
				break;
			}
			else {
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
	cout << "Please enter your passphrase: ";
	char c = 0;
	while (true) {
		c = _getch();
		if (c == 0xD) break;
		result += c;
	}
	cout << endl;
	return result;
}

string toHex(uint32_t num) {
	stringstream buffer;
	buffer << hex << num;
	string result(buffer.str());
	return result;
}

string decryptFile(PasswordDatabase &db, uint32_t &key, string &passphrase, string encryptedData) {
	string result = "";
	while (true) {
		key = getKeyFromUser();
		passphrase = getPassphraseFromUser();
		cout << "Decrypting file..." << endl;
		result=encryptDecrypt(encryptedData, key, passphrase);
		if (result.substr(0, 26) == "ABCDEFGHIJKLMNOPQRSTUVWXYZ") break; //success
		cout << "Error: incorrect key or passphrase. Please try again." << endl;
	}
	cout << "Decryption successful." << endl;
	return result;
}

void writeAndEncryptFile(PasswordDatabase db, uint32_t key, string passphrase, string filename) {
	cout << "Saving and encrypting file..." << endl;
	string data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
	for (int i = 0; i < db.getLabels().size(); i++) {
		data += db.getLabels()[i] + "\n";
		data += db.getPasswords()[i];
		if (i != db.getLabels().size() - 1) data += "\n";
	}
	data = encryptDecrypt(data, key, passphrase);

	ofstream passwordFile(filename, ofstream::out);
	passwordFile << data;
	passwordFile.close();

	cout << "Done. " << endl;
}

int main(int argc, char* argv[]) {

	string nameOfWordList = "wordlist.txt";
	string nameOfPasswords = "passwords.txt";

	vector<string> wordlist;
	stringstream encryptedPasswordsData;

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
	passwordsFile.open(nameOfPasswords);
	if (!passwordsFile) {
		cout << "Unable to open passwords file. Entering first time setup." << endl;
		cout << "Creating key..." << endl;
		key = generateNewKey();
		cout << "Done creating key. Creating passphrase..." << endl;
		passphrase = generatePassphrase(wordlist);
		cout << "Passphrase created." << endl
			<< endl << "-----------------------------" << endl
			<< "KEY: " << setw(8) << setfill('0') << right << hex << key << endl
			<< "PASSPHRASE: " << passphrase << endl
			<< "-----------------------------" << endl << endl
			<< "***NOTE: These are required to access your passwords." << endl
			<< "Recovery of any password is not possible if you lose these." << endl
			<< "Type \"yes\" if you acknowledge this notice: ";
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
		encryptedPasswordsData << passwordsFile.rdbuf();
		string decrypted=decryptFile(db, key, passphrase, encryptedPasswordsData.str());
		passwordsFile.close();
		db = PasswordDatabase(decrypted);
	}
	
	cout << endl;
	string greeting = "Welcome to diceware password generator and manager!\nSoftware by Metgame (Austin Scott) V1.0";
	printBox(greeting);
	cout << endl;

	bool keeplooping = true;
	while (keeplooping) {
		cout << "Menu:\n\t1. Add new passphrase.\n\t2. View existing passphrase.\n\t3. Edit existing passphrases.\n\t4. Edit database key/passphrase.\n\t5. Quit" << endl << endl;
		int choice = 0;
		cout << "Choose option: ";
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

			break;
		case 4:

			break;
		case 5:
			writeAndEncryptFile(db, key, passphrase, nameOfPasswords);
			keeplooping = false;
			break;
		default:
			cout << "Error: invalid choice. Please choose again." << endl;
		}
	}

}