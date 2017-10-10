#pragma once
/*
Encryption.h - where all the core functions used in the stream cypher are stored
*/
#include <string>
#include <cstdint>

#include "picosha2.h"

#include "Utils.h"

using namespace std;

std::string encryptDecrypt(std::string data, uint32_t salt, uint32_t key, std::string passphrase);
std::string saltString(std::string str, uint32_t salt);
std::string hashString(std::string str);
