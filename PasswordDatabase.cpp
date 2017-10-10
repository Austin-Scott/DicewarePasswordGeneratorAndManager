#include "PasswordDatabase.h"


PasswordDatabase::PasswordDatabase() {
	unsavedChanges = false;
}

PasswordDatabase::PasswordDatabase(std::string decryptedFile) {
	unsavedChanges = false;
	stringstream buffer(decryptedFile);
	std::string line = "";
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

vector<std::string> PasswordDatabase::getLabels() {
	return labels;
}

vector<std::string> PasswordDatabase::getPasswords() {
	return passwords;
}

vector<std::string> PasswordDatabase::getDates() {
	return dates;
}

vector<std::string> PasswordDatabase::getNotes() {
	return notes;
}

bool PasswordDatabase::hasUnsavedChanges() {
	return unsavedChanges;
}

void PasswordDatabase::flagUnsavedChanges() {
	unsavedChanges = true;
}

void PasswordDatabase::printCensoredEntry(int id) {

	cout << labels[id] << " (" << dates[id] << ")" << endl;
}

void PasswordDatabase::printEntry(int id) {

	std::string content = "---Password for " + labels[id] + "---\n\n     "
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

void PasswordDatabase::addNewPassphrase(vector<std::string>& wordlist) {
	cout << "What is the name of the site this passphrase is for?>" << endl;
	std::string name = "";
	getline(cin, name, '\n');
	std::string passphrase = generatePassphrase(wordlist);

	labels.push_back(name);
	passwords.push_back(passphrase);

	notes.emplace_back("This passphrase has no notes.");

	dates.push_back(getCurrentDateTime());

	unsavedChanges = true;
	cout << "Passphrase added." << endl;
}

void PasswordDatabase::editEntry(int id, vector<std::string>& wordlist) {
	while (true) {
		cout << "Edit options for " + labels[id] + ":\n\t1. Change label.\n\t2. Change passphrase.\n\t3. Delete entry.\n\t4. Edit password notes\n\t5. Cancel edit." << endl << endl;
		int choice = 0;
		cout << "Enter selection> ";
		cin >> choice;
		flushCin();
		if (choice == 1) {
			cout << "Please enter a new name for " + labels[id] + ">" << endl;
			editString(labels[id]);
			cout << "Name changed to: " + labels[id] << endl;
			unsavedChanges = true;

			break;
		}
		else if (choice == 2) {
			cout << "Changing passphrase for " + labels[id] + "..." << endl;
			passwords[id] = generatePassphrase(wordlist);
			cout << "Passphrase changed." << endl;
			dates[id] = getCurrentDateTime();
			unsavedChanges = true;
			break;
		}
		else if (choice == 3) {
			cout << "Are you sure you want to delete " + labels[id] + "? (yes/no)" << endl;



			std::string input = "";
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
			cout << "Editing notes for " << labels[id] << ">" << endl;
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
