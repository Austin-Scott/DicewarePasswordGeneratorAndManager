# Diceware Passphrase Generator/Manager
Simple console based diceware passphrase generator and manager.

# About
You can read about what Diceware passphrases are and why you should use them [here](https://www.eff.org/deeplinks/2016/07/new-wordlists-random-passphrases).
The purpose of this application is to help you generate and remember these passphrases.

This application uses two text files to do its job: a "wordlist.txt" and a "passwords.txt" (These names can be changed using cmd-line arguments if you wish).
The wordlist file contains a list of line seperated words to be used in creating new passphrases. The default wordlist was downloaded and then modifed from [here](https://www.eff.org/files/2016/07/18/eff_large_wordlist.txt).
The passwords file stores an encrypted database of passphrases that you have created using this application in case you forget one and would like to look it up.
In order to decrypt this file you will need to remember a simple custom passphrase. These is created when you first open the application.

# Usage
The first time you open this application it will fail to find a "passwords.txt" file and it will enter first time setup to create it. 
This setup will step you through the process of creating a passphrase that will be used whenever you restart the application. **Do not forget this passphrase because there is currently no way to recover it if you forget it**
although you may change your passphrase any time you want from inside the application. You should now be looking at the main menu. 

When you are at the main menu you may select options by typing their number followed by enter. Your options include:
1. Create a new passphrase.
2. View a previously created passphrase.
3. Edit (change passphrase, rename, edit notes, or delete) a previously created passphrase.
4. Change the password database's passprase.
5. Save and quit.

Please note that you must have at least one entry in your password database before you will be allowed to save it.

Whenever you are creating a passphrase the program will ask you if you would like to append anything to the end of a passphrase. This is for finicky websites that require special characters in their passwords such as numbers and puncuation. You may add any required characters here.

## Generating new passphrases
Whenever you are generating a new passphrase you will first be prompted to enter the length of your new passphrase in words. The longer your passphrase the more secure it will be. Next you will be asked if you would like to append any special characters to the end of your passphrase. Press "y" for yes or "n" for no. Doing so will make your passphrase more secure but is not required for a secure passphrase. This feature is intended for websites that require special characters in their passwords before they will except them. Finally, you should now be presented with a menu giving you options on how you would like the program to choose which words will be used in your passphrase:
1. Hardware based true random number generator.
2. Real world dice. 
3. Manual entry of ranged numbers.
4. Pseudo-random number generator.

Number one is the easiest and most secure option if it is available to you. Number two allows you to use physical die to select which words should be used. This is also a great option especially if your wordlist has a length of 6^n (such as the default word-list) and you have a physical die to use. Number three allows you to use your own random number generator to choose words for your passphrase by typing in ranged numbers outputted from your own source of randomness. And finally, number 4 should only be used as a last resort because it relies on a weak pseudo-random number generator. If you do go with this option you will be presented with a menu on how you would like to seed the pseudo-random number generator. 

After you have generated your custom passphrase you will be presented with a screen filled with some statistics about your passphrase and the passphrase will be copied to your clipboard for convience in pasting it into a website's new password prompt. When you are done press enter and the passphrase will be removed from your clipboard and the new passphrase wizard will close.

# Dependencies
This project uses [libsodium](https://download.libsodium.org/doc/) for all of its encryption needs so if you are trying to build it from source you will need to link to this library.
