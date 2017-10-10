#include "Utils.h"

#ifdef _WIN32
	void toClipboard(const std::string & s) { //Modified from: http://www.cplusplus.com/forum/general/48837/
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
#endif

int toInt(std::string value) {
	std::stringstream buffer;
	buffer << value;
	int result;
	buffer >> result;
	return result;
}

void editString(std::string & stringToEdit) {
	std::cout << stringToEdit;
	while (true) {
		char c = _getch();
		if (c == 0xD) { //If enter is pressed stop editing
			std::cout << std::endl;
			break;
		}
		else if (c == 0x8) { //If backspace is pressed
			if (stringToEdit.length() > 0) {
				stringToEdit.pop_back();
				std::cout << "\b \b"; //Move the cursor back one, overwrite star with space, then move back again
			}
		}
		else if (c >= 0x20 && c <= 0x7E) { //If printable character is typed add to end of std::string
			stringToEdit += c;
			std::cout << c;
		}
	}
}

void printBox(std::string value) {
	const char topLeft = 201;
	const char topRight = 187;
	const char bottomLeft = 200;
	const char bottomRight = 188;
	const char topBottom = 205;
	const char leftRight = 186;

	std::stringstream buffer;
	buffer << value;
	std::vector<std::string> lines;
	while (!buffer.eof()) {
		std::string line = "";
		getline(buffer, line);
		lines.push_back(line);
	}

	int maxLength = 0;
	for (std::string s : lines) {
		if (s.length() > maxLength) maxLength = s.length();
	}

	std::string floor = "";
	floor.assign(maxLength, topBottom);

	std::string top = "";
	top.push_back(topLeft);
	top += floor;
	top.push_back(topRight);

	std::cout << top << std::endl;

	for (std::string s : lines) {
		std::cout << leftRight << std::setw(maxLength) << std::setfill(' ') << std::left << s << leftRight << std::endl;
	}

	std::string bottom = "";
	bottom.push_back(bottomLeft);
	bottom += floor;
	bottom.push_back(bottomRight);

	std::cout << bottom << std::endl;

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
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

std::string toHex(uint32_t num) {
	std::stringstream buffer;
	buffer << std::hex << num;
	std::string result(buffer.str());
	std::cout << std::dec;
	return result;
}

std::string getCurrentDateTime() {
	char buffer[80];
	time_t raw;
	struct tm * timeinfo;
	time(&raw);
	timeinfo = localtime(&raw);
	strftime(buffer, 80, "%B %e %Y %I:%M:%S %p", timeinfo);
	std::string result = std::string(buffer);
	return result;
}