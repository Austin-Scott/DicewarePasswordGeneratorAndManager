#pragma once
/*
Application.h - for functions that are used for user interaction in the application
*/

#include <iostream>
#include <string>
#include <cstdint>
#include <random>
#include <fstream>

#include "Utils.h"
#include "PasswordDatabase.h"
#include "Encryption.h"

using namespace std;

string generatePassphrase(vector<string> &wordlist);
int searchForEntry(PasswordDatabase &db);
uint32_t generateNewKey(string name);
int getRandomNumberFromDie(int max);
uint32_t getKeyFromUser();
string getPassphraseFromUser();
string decryptFile(PasswordDatabase &db, uint32_t &key, string &passphrase, uint32_t salt, string hash, string encryptedData);
void writeAndEncryptFile(PasswordDatabase db, uint32_t key, uint32_t salt, string passphrase, string filename);
void changeKeyPass(PasswordDatabase &db, uint32_t &salt, uint32_t &key, string &passphrase, vector<string> &wordlist);

