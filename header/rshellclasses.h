#ifndef RSHELLCLASSES_H
#define RSHELLCLASSES_H

#include <vector>
#include <string>
#include <stack>
#include <deque>
#include <cassert>
#include <unordered_map>
#include <stdlib.h>
#include <sys/stat.h>
#include "rshellutils.h"
#include "executeCharArray.h"
#include "convertVectorToCharArray.h"

using namespace std;

int GLOBAL_EXIT_STATUS = 0; // When set to 1, stop program execution.
						    // This is used for proper handling of exit() -
							// wasn't sure how to handle it otherwise since
							// Tokens have no access to RShell, and we're using composite pattern.

class Token {
    public:
        Token() {}
        virtual ~Token() {}

        virtual int execute() = 0;
        virtual string stringify() { return joinVector(content, ' '); }

        virtual bool hasChildren() { return ((leftChild != nullptr) || (rightChild != nullptr)); }
		bool operator==(Token const &rhs) const {
			return (this->content == rhs.content);
		}

        // Member variables
        vector<string> content;
        Token* leftChild = nullptr;
        Token* rightChild = nullptr;
        int status = -2; // Current exit status of Token.
		// Meanings of status:
		// -3: Subcommand segfaulted when we tried to run it with execvp
		// -2: Token has not run
		// -1: Subcommand not found
		// 0: Subcommand ran successfully
		// anything else: Subcommand failed
		bool isOperator;
};

class Subcommand : public Token {
    public:
        virtual ~Subcommand() {}
        Subcommand(vector<string> V) { 
			content = V; 
			isOperator = false;
		}
        virtual string stringify() { return /*"Subcommand: " + */joinVector(content, ' '); }

		bool operator==(Subcommand const rhs) const {
			return (this->content == rhs.content);
		}
        
		bool test()
		{
			if (content[1] == "-e")
			{
				struct stat check;
				return (stat(content[2].c_str(), &check) == 0);
				//checks if the file/directory exists
			}
			else if (content[1] == "-f")
			{
				//checks if the file/directory exists and is a regular file
				struct stat check;
				if (stat(content[2].c_str(), &check) == 0)
				{
					if (check.st_mode & S_IFREG)
					{
						return true; 
					}
					else{
						return false;}
				}

		
			}
			else if (content[1] == "-d")
			{
				struct stat check;
				if (stat(content[2].c_str(),&check) == 0)
				{
					if (check.st_mode & S_IFDIR)
					{
						return true; 
					}
					else{
						return false;}

				}
				//checks if the file/directory exists and is a directory
			}
			else 
			{
				struct stat check;
				return (stat(content[1].c_str(), &check) == 0);
				//checks if the file/directory exists
			 
			}
		}

		virtual int execute() {
			if (GLOBAL_EXIT_STATUS == 1) {
				// Do nothing
				return -2;
			}
            else if (content[0] == "exit") {
				GLOBAL_EXIT_STATUS = 1;
				status = 0;
				return status;
			} else if (content[0] == "test") {
				if (test() == true) { 
					cout << "(True)" << endl;
					status = 0;
				} else { 
					cout << "(False)" << endl;
					status = 1;
				}
				return status;
			} else { 
				char** chararr = convertVectorToCharArray(content);
				status = executeCharArray(chararr);
				
				if (status == -1) {
					cout << "RSHELL: Command not found!" << endl;
				}

				for (int i = 0; i < content.size()+1; i++) {
					delete[] chararr[i];
				}
				delete[] chararr;

				return status;
			}
		}

};

class AndToken: public Token {
	public: 
			
		AndToken(vector<string> V) { 
			content = V; 
			isOperator = true;
		}

        virtual string stringify() { return "AndToken: " + joinVector(content, ' '); }

		bool operator==(AndToken const rhs) const {
			return (this->content == rhs.content);
		}

		virtual int execute() {
			int statusLeft, statusRight = -2;
			statusLeft = leftChild->execute();
			
			if (statusLeft == 0) {
				statusRight = rightChild->execute();
				if (statusRight == 0)
				{
					this->status = 0;
				}
				else 
				{ 
					this->status = 1;
				}
			}
			else 
			{
				this->status = 1;
			}

			return this->status; 
		}

};

class OrToken: public Token {
	public: 
		OrToken(vector<string> V) { 
			content = V; 
			isOperator = true;
		}

        virtual string stringify() { return "OrToken: " + joinVector(content, ' '); }

		bool operator==(OrToken const rhs) const {
			return (this->content == rhs.content);
		}

        virtual int execute() {
            int statusLeft, statusRight = -2;
            statusLeft = leftChild->execute();
            
            if (statusLeft != 0) 
			{
                statusRight = rightChild->execute();
				if (statusRight == 0)
				{
					this->status = 0;
				}
				else 
				{ 
					this->status = 1;
				}
			}
			else 
			{
				this->status = 0;
			}

			return this->status; 
		}
};

class SemiToken: public Token {
	public:
		SemiToken(vector<string> V) {
			content = V; 
			isOperator = true;
		}
		
        virtual string stringify() { return "SemiToken: " + joinVector(content, ' '); }

		bool operator==(SemiToken const rhs) const {
			return (this->content == rhs.content); 
		}

		virtual int execute() { 
			leftChild->execute();
			this->status = rightChild->execute();
			return this->status;
		}
};

class ParenthesisToken : public Token {
	public:
		// Acts like a decorator
		ParenthesisToken(deque<Token*> inside) {
			interior = inside;
			isOperator = false;
		}

        virtual string stringify() { 
			vector<string> outputV;
			outputV.push_back("ParenthesisToken:");
			string indent(4, ' ');

			for (Token* t : interior) {
				outputV.push_back(indent + t->stringify());
			}
			outputV.push_back("}");
			return joinVector(outputV, "\n");
		}

		virtual int execute() { // Placeholder
			return 0;
		}

	//	virtual int execute() {
	//		shuntingExecute(interior);
	//	}

		deque<Token*> interior;
};

class TestToken : public Token {
	public:
		// Holds stuff from [   ]
		// So this->content = {"-e", "path/to/file"} or something similar.
		
		TestToken(vector<string> V) {
			content = V;
			isOperator = false;
		}

		virtual int execute()
		{
			if (content[0] == "-e")
			{
				struct stat check;
				
				if (stat(content[1].c_str(), &check) == 0) 
				{
					cout << "(True)" << endl;
					this->status = 0;
					return this->status;
				}
				else 
				{	
					cout << "(False)" << endl;
					this->status = 1;
					return this->status;
				}
				//checks if the file/directory exists
			}
			else if (content[0] == "-f")
			{
				//checks if the file/directory exists and is a regular file
				struct stat check;
				if (stat(content[1].c_str(), &check) == 0)
				{
					if (check.st_mode & S_IFREG)
					{
						cout << "(True)" << endl;
						this->status = 0;
						return this->status; 
					}
					else
					{
						cout << "(False)" << endl;
						this->status = 1;
						return this->status;
					}
				}
			}
			else if (content[0] == "-d")
			{
				struct stat check;
				if (stat(content[1].c_str(),&check) == 0)
				{
					if (check.st_mode & S_IFDIR)
					{
						cout << "(True)" << endl;
						this->status = 0;
						return this->status; 
					}
					else 
					{
						cout << "(False)" << endl;
						this->status = 1;
						return this->status;
					}

				}
				//checks if the file/directory exists and is a directory
			}
			else 
			{
				struct stat check;
				
				if (stat(content[0].c_str(), &check) == 0) 
				{
					cout << "(True)" << endl;
					this->status = 0;
					return this->status;
				}
				else 
				{
					cout << "(False)" << endl;
					this->status = 1;
					return this->status;
				}	
				//checks if the file/directory exists
			}
		}
        virtual string stringify() { return "TestToken: " + joinVector(content, ' '); }
};

class StorageToken : public Token {
	public:
		StorageToken(int s) {
			isOperator = false;
			status = s;
		}
		virtual int execute() {
			return status;
		}
		virtual string stringify() {
			return "StorageToken."; // Should never be printed
		}
};

class RShell {
    public:
		RShell() {}
		RShell(bool b) {
			DEBUG = b;
		}
		RShell(string filename) {
			// TODO: Make this take in .rshellrc and set up configData accordingly
		}
		virtual ~RShell() {
			deconstructDeque();
		}

		void makeCommandDeque(string);
		int executeCommandDeque();

		void deconstructDeque() {
			if (commandDeque.size() > 0) {
				for (Token* t : this->commandDeque) {
					delete t;
				}
			}
		}
		
		// Member variables
		deque<Token*> commandDeque;
		unordered_map <string, string> configData;
		bool DEBUG = true;
   
		bool checkBuiltin(vector<string>);
		deque<Token*> shuntingYardConstruct(string);
		int shuntingExecute(deque<Token*>);
		int findClose(const string&,int,char); 
};

#endif
