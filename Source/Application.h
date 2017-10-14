#pragma once
/*
Application.h - for functions that are used for user interaction in the application
*/

#include <iostream>
#include <string>
#include <cstdint>
#include <random>
#include <fstream>
#include <vector>

#include "PasswordDatabase.h"
#include "Encryption.h"

class PasswordDatabase;

std::string generatePassphrase(std::vector<std::string>& wordlist);
int searchForEntry(PasswordDatabase &db);
uint32_t generateNewKey(std::string name);
int getRandomNumberFromDie(int max);
uint32_t getKeyFromUser();
std::string getPassphraseFromUser();
std::string decryptFile(PasswordDatabase &db, std::string &passphrase, uint32_t salt, unsigned char* nonce, std::string encryptedData);
void writeAndEncryptFile(PasswordDatabase db, const uint32_t currentVersion, uint32_t salt, unsigned char* nonce, std::string passphrase, std::string filename);
void changePassphrase(PasswordDatabase &db, uint32_t &salt, std::string &passphrase, std::vector<std::string> &wordlist);
bool parseCmdArguments(int argc, char **argv, std::string &nameOfWordList, std::string &nameOfPasswords);
bool loadWordlist(std::string nameOfWordList, std::vector<std::string> &wordlist);
void firstTimeSetup(std::string &passphrase, std::vector<std::string> &wordlist);
