#include "PasswordDatabase.h"


PasswordDatabase::PasswordDatabase() {
	unsavedChanges = false;
}

PasswordDatabase::PasswordDatabase(std::string decryptedFile, uint32_t version) {
	unsavedChanges = false;
	std::stringstream buffer(decryptedFile);
	std::string line = "";

	if (version >= 5) {
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
}

std::vector<std::string> PasswordDatabase::getLabels() {
	return labels;
}

std::vector<std::string> PasswordDatabase::getPasswords() {
	return passwords;
}

std::vector<std::string> PasswordDatabase::getDates() {
	return dates;
}

std::vector<std::string> PasswordDatabase::getNotes() {
	return notes;
}

bool PasswordDatabase::hasUnsavedChanges() {
	return unsavedChanges;
}

void PasswordDatabase::flagUnsavedChanges() {
	unsavedChanges = true;
}

void PasswordDatabase::printCensoredEntry(int id) {

	std::cout << labels[id] << " (" << dates[id] << ")" << std::endl;
}

void PasswordDatabase::printEntry(int id) {

	std::string content = "---Password for " + labels[id] + "---\n\n     "
		+ passwords[id]
		+ "     \n\n---Password creation time stamp---\n\n     "
		+ dates[id]
		+ "     \n\n---Notes---\n\n     "
		+ notes[id]
		+ "     \n";
	std::cout << std::endl;
	printBox(content);
	std::cout << std::endl;

}

void PasswordDatabase::addNewPassphrase(std::vector<std::string>& wordlist) {
	std::cout << "What is the name of the site this passphrase is for?>" << std::endl;
	std::string name = "";
	getline(std::cin, name, '\n');
	std::string passphrase = generatePassphrase(wordlist);

	labels.push_back(name);
	passwords.push_back(passphrase);

	notes.emplace_back("This passphrase has no notes.");

	dates.push_back(getCurrentDateTime());

	unsavedChanges = true;
	std::cout << "Passphrase added." << std::endl;
}

void PasswordDatabase::editEntry(int id, std::vector<std::string>& wordlist) {
	while (true) {
		std::cout << "Edit options for " + labels[id] + ":\n\t1. Change label.\n\t2. Change passphrase.\n\t3. Delete entry.\n\t4. Edit password notes\n\t5. Cancel edit." << std::endl << std::endl;
		int choice = 0;
		std::cout << "Enter selection> ";
		cin >> choice;
		flushCin();
		if (choice == 1) {
			std::cout << "Please enter a new name for " + labels[id] + ">" << std::endl;
			editString(labels[id]);
			std::cout << "Name changed to: " + labels[id] << std::endl;
			unsavedChanges = true;

			break;
		}
		else if (choice == 2) {
			std::cout << "Changing passphrase for " + labels[id] + "..." << std::endl;
			passwords[id] = generatePassphrase(wordlist);
			std::cout << "Passphrase changed." << std::endl;
			dates[id] = getCurrentDateTime();
			unsavedChanges = true;
			break;
		}
		else if (choice == 3) {
			std::cout << "Are you sure you want to delete " + labels[id] + "? (yes/no)" << std::endl;



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
				std::cout << "Entry deleted." << std::endl;
				unsavedChanges = true;
				break;
			}


		}
		else if (choice == 4) {
			std::cout << "Editing notes for " << labels[id] << ">" << std::endl;
			editString(notes[id]);
			std::cout << "Notes updated." << std::endl;
			unsavedChanges = true;
			break;
		}
		else if (choice == 5) {

			break;
		}
		else {
			std::cout << "Error: invalid choice. Please try again." << std::endl;
		}
	}
}
