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

std::string encryptString(std::string message, uint32_t salt, const unsigned char *npub, std::string passphrase)
{
	std::string saltedPassphrase = saltString(passphrase, salt);
	std::string secretKey = hashString(saltedPassphrase);
	unsigned long long mlen = message.length();
	unsigned long long clen = 0;
	unsigned char* cipertext = new unsigned char[mlen + crypto_aead_xchacha20poly1305_ietf_ABYTES];

	crypto_aead_xchacha20poly1305_ietf_encrypt(cipertext, &clen, (const unsigned char*)message.c_str(), mlen, NULL, 0, NULL, npub, (unsigned char*)secretKey.c_str());

	std::string result = std::string((const char*)cipertext, clen);

	delete[] cipertext;

	return result;
}

bool decryptString(std::string ciphertext, std::string & message, uint32_t salt, const unsigned char *npub, std::string passphrase)
{
	std::string saltedPassphrase = saltString(passphrase, salt);
	std::string secretKey = hashString(saltedPassphrase);
	unsigned long long clen = ciphertext.length();
	unsigned char* m = new unsigned char[clen - crypto_aead_xchacha20poly1305_ietf_ABYTES];
	unsigned long long mlen = 0;
	bool result;

	if (crypto_aead_xchacha20poly1305_ietf_decrypt(m, &mlen, NULL, (unsigned char*)ciphertext.c_str(), clen, NULL, 0, npub, (unsigned char*)secretKey.c_str()) == 0) {
		//Success!
		message = std::string((char *)m, mlen);
		result = true;
	}
	else {
		//Failed
		result = false;
	}

	delete[] m;
	
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
