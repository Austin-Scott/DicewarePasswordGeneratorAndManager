#include "Encryption.h"

string encryptDecrypt(string data, uint32_t salt, uint32_t key, string passphrase) {
	string saltedPassphrase = saltString(passphrase, salt);
	string hashedPassphrase = hashString(saltedPassphrase);

	string result = data;
	seedGenerator(key);
	if (passphrase.length() != 0) {
		for (int i = 0; i < result.length(); i++) { //For every byte in data
			for (int j = 0; j < 8; j++) { //For every bit in each byte
				int currentHashByte = random() % 32;
				int currentHashBit = random() % 8;
				xorSetBit(result[i], getBit(hashedPassphrase[currentHashByte], currentHashBit), j);
			}
		}
	}
	return result;
}

string saltString(string str, uint32_t salt)
{
	string result = str;
	for (int i = 0; i < result.length(); i++) {
		result[i] ^= getByte(salt, i % 4);
	}
	return result;
}

string hashString(string str)
{
	vector<char> hash(32);
	picosha2::hash256(str.begin(), str.end(), hash.begin(), hash.end());
	string result = "";
	for (int i = 0; i < 32; i++) result += hash[i];
	return result;
}
