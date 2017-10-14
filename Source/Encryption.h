#pragma once
/*
Encryption.h - where all the core functions used in the stream cypher are stored
*/
#include <string>
#include <cstdint>

#define SODIUM_STATIC 1
#define SODIUM_EXPORT
#include <sodium.h>
#include "picosha2.h"

#include "Utils.h"

using namespace std;

[[deprecated("Proven unsecure. Do not use this function.")]]
std::string encryptDecrypt(std::string data, uint32_t salt, uint32_t key, std::string passphrase);

std::string encryptString(std::string message, uint32_t salt, const unsigned char *npub, std::string passphrase);
bool decryptString(std::string ciphertext, std::string &message, uint32_t salt, const unsigned char *npub, std::string passphrase);

std::string saltString(std::string str, uint32_t salt);
std::string hashString(std::string str);
