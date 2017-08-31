# Diceware Passphrase Generator/Manager
Simple console based diceware passphrase generator and manager.

# About
You can read about what Diceware passphrases are and why you should use them [here](https://www.eff.org/deeplinks/2016/07/new-wordlists-random-passphrases).
The purpose of this application is to help you generate and remember these passphrases.

This application uses two text files to do its job: a "wordlist.txt" and a "passwords.txt" (These names can be changed using cmd-line arguments if you wish).
The wordlist file contains a list of line seperated words to be used in creating new passphrases. The default wordlist was downloaded and then modifed from [here](https://www.eff.org/files/2016/07/18/eff_large_wordlist.txt).
The passwords file stores an encrypted database of passphrases that you have created using this application in case you forget one and would like to look it up.
In order to decrypt this file you will need to remember a 8 digit hexadecimal key and a short custom passphrase. These are both entered when you open the application.

# Usage
The first time you open this application it will fail to find a "passwords.txt" file and it will enter first time setup to create it. 
This setup will step you through the process of creating a key and passphrase that will be used whenever you restart the application. **Do not forget either of these items because there is currently no way to recover them if you forget them**
although you may change them from inside the application at a later date. You should now be looking at the main menu. 

When you are at the main menu you may select options by typing their number followed by enter. Your options include:
1. Create a new passphrase.
2. View a previously created passphrase.
3. Edit (change passphrase, rename, or delete) a previously created passphrase.
4. Change the password database's key or passprase.
5. Save and quit.

Whenever you are creating a passphrase the program will ask you if you would like to append anything to the end of a passphrase. This is for finicky websites that require special characters in their passwords such as numbers and puncuation. You may add any required characters here.
