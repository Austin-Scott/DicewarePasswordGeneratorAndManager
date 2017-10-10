#include "Utils.h"

int toInt(string value) {
	stringstream buffer;
	buffer << value;
	int result;
	buffer >> result;
	return result;
}

void editString(string & stringToEdit) {
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

uint32_t random(void) { //RANDOM_MAX is 32767
	nextSeed = nextSeed * 1103515245 + 12345;
	return (uint32_t)(nextSeed / 65536) % 32768;
}

void seedGenerator(uint32_t seed) {
	nextSeed = seed;
}

bool getBit(char from, int index) { //index starts at zero as the least significant bit
	return (from >> index) & 0x1;
}

void xorSetBit(char & to, bool value, int index) { //index starts at zero as the least significant bit
	to ^= (value << index);
}

char getByte(uint32_t from, int index) //index starts at zero at the least significant byte
{
	return (from >> (index * 8)) & 0xff;
}

void flushCin() {
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

string toHex(uint32_t num) {
	stringstream buffer;
	buffer << hex << num;
	string result(buffer.str());
	cout << dec;
	return result;
}
