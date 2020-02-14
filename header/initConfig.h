#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "splitOnChar.h"

// NOT CURRENTLY USED; ASSIGNMENT 3 SKETCH WORK

using namespace std;

unordered_map<string, bool> initConfig(string filename) {
	// Implementation:
	// When checking whether a setting is true or not, make sure to
	//    store the false result in config if so. That allows for the
	//    developer to simply call config["setting"] instead of having to
	//    check if it exists in the map first.

	unordered_map<string, bool> config;
	
	ifstream inFile = open(filename);
	
	string currLine;
	while (getline(inFile, currLine)) {
		if (currLine[0] == "#") {
			continue;
		}
		
		vector<string> words = splitOnChar(currLine, ' ');
		if (words[0] == "alias") {
			// eg "alias cs=cowsay Welcome programmer! Good luck on dev!"
			vector<string> leftright = splitOnChar(currLine, '=');
			// TODO later; aliases will be tricky to store in memory. May need a stringify rshellutil
		}
	}

	inFile.close();
	return config;
}
