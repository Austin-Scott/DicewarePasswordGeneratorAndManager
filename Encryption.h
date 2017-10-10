#pragma once
/*
Encryption.h - where all the core functions used in the stream cypher are stored
*/
#include <string>
#include <cstdint>

#include "picosha2.h"

#include "Utils.h"

using namespace std;

string encryptDecrypt(string data, uint32_t salt, uint32_t key, string passphrase);
string saltString(string str, uint32_t salt);
string hashString(string str);
