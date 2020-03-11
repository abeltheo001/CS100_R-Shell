#ifndef RSHELLCLASSES_H
#define RSHELLCLASSES_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <deque>
#include <cassert>
#include <unordered_map>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <cstring>
#include <thread>
#include <chrono>
#include <fcntl.h>
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
	bool isOperator;
        Token* leftChild = nullptr;
        Token* rightChild = nullptr;
        int status = -2; // Current exit status of Token.
		// Meanings of status:
		// -3: Subcommand segfaulted when we tried to run it with execvp
		// -2: Token has not run
		// -1: Subcommand not found
		// 0: Subcommand ran successfully
		// anything else: Subcommand failed
};

class Subcommand : public Token {
    public:
        virtual ~Subcommand() {}
        Subcommand(vector<string> V) { 
			content = V; 
			isOperator = false;
		}
        virtual string stringify() { return "Subcommand: \"" + joinVector(content, ' ') + "\""; }

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
						return false;
					}
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
						return false;
					}

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

        virtual string stringify() { return "AndToken: \"" + joinVector(content, ' ') + '\"'; }

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

        virtual string stringify() { return "OrToken: \"" + joinVector(content, ' ') + '\"'; }

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
		
        virtual string stringify() { return "SemiToken: \"" + joinVector(content, ' ') + "\""; }

		bool operator==(SemiToken const rhs) const {
			return (this->content == rhs.content); 
		}

		virtual int execute() { 
			leftChild->execute();
			this->status = rightChild->execute();
			return this->status;
		}
};

class TestToken : public Token {
	public:
		// Holds stuff from [   ]
		// So this->content = {"-e", "path/to/file"} or something similar.
		
		TestToken(vector<string> V) {
			content = V;
			isOperator = false;
		}
		
		virtual string stringify() { return "TestToken: \"" + joinVector(content, ' ') + "\""; }

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
			return "StorageToken: " + to_string(status);
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
		bool DEBUG = false;
   
		bool checkBuiltin(vector<string>);
		deque<Token*> shuntingYardConstruct(string);
		int shuntingExecute(deque<Token*>);
		int findClose(const string&,int,char); 
};

class ParenthesisToken : public Token {
	public:
		// Acts like a decorator
		ParenthesisToken(deque<Token*> inside) {
			interior = inside;
			isOperator = false;
		}

		virtual ~ParenthesisToken() {
			if (this->interior.size() > 0) {
				for (Token* t : this->interior) {
					delete t;
				}
				interior.clear();
			}
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

		virtual int execute() {
			RShell temp = RShell(false);
			this->status = temp.shuntingExecute(interior);
			return status;
		}

		deque<Token*> interior;
};

//Assignment 4 Classes 
//
// Check for inputredirection. 
// <, >, >>, and |. 
		
// < - Accept input from a file or given subcommand. Store the contents of the command and	
// insert the result into the desired location. 
//
// > - Redirect current subcommand into a file. Create the file if it dosen't exist. If it already
// exists, Empty the file's exisiting content before appending the new information.
//
// >> - Redirect subcommand to a file and append it's result to a file by the given name. 
// Create the file if it dosen't exist. 
//
//> and >> 
class RedirectOutToken : public Token {
	public: 
		RedirectOutToken(vector<string> V) {
			content = V; 
			isOperator = true; 
		}
		virtual string stringify() { return "RedirectOutToken (>, >>): \"" + joinVector(content, ' ') + "\""; }
		virtual int execute()
		{
			//Placeholder for result from command. Use dup to run command. 
		 	int command = leftChild->execute();
			string fileName = rightChild->content[0];
			string testCommand = "a";
			char buffer[20];
			
			if (content[0] == ">") {
				int file_fd = open(fileName.c_str(), O_RDWR|O_CREAT|O_TRUNC);
				if (file_fd == -1) {
					cout << "Error found" << endl;
					return -1;
				}
				write (file_fd, testCommand.c_str(), testCommand.size()+1);
				close (file_fd);
			}
			else if (content[0] == ">>") {
				int file_fd = open(fileName.c_str(), O_RDWR|O_CREAT|O_APPEND);
				if (file_fd == -1) {
					cout << "Error found" << endl;
					return -1; 
				}
				write (file_fd, testCommand.c_str(), testCommand.size()+1);
				close (file_fd);
			}
			else 
			{
				cout << "Error found" << endl;
				return -1;
			}

			return 0; 
		}
};

class RedirectInputToken : public Token {
	public:
		RedirectInputToken(vector<string> V) {
			content = V;
			isOperator = true; 
		}
		virtual string stringify() { return "RedirectInputToken (<): \"" + joinVector(content, ' ') + "\""; }
		virtual int execute() {
			return 0; // Needs to have a default value to avoid being pure virtual
		};

};

class PipeToken : public Token {
	public:
		PipeToken(vector<string> V) {
			content = V;
			isOperator = true; 
		}
		virtual string stringify() { return "PipeToken (|): \"" + joinVector(content, ' ') + "\""; }
		virtual int execute() {
			string leftCommand = leftChild -> content[0];
			string rightCommand = rightChild -> content[0];
			bool flag = true;
			string r = "r";
			string w = "w";
			
			const int PATH_MX = 420;
			char buffer[PATH_MX];
			char buffer2[PATH_MX];
			
			memset(buffer,'\0',PATH_MX);
			memset(buffer2, '\0',PATH_MX);
			
			FILE *in_pipe = popen(leftCommand.c_str(), r.c_str());
			FILE *out_pipe = popen(rightCommand.c_str(), w.c_str());
			
			if ((in_pipe = nullptr) && (out_pipe == nullptr)) {
				flag = false;
				cout << "Piping error, check input";
			}

			while (fgets(buffer,PATH_MX,in_pipe) != nullptr) {
				fputs(buffer,out_pipe);
			}
			
			pclose(in_pipe);
			pclose(out_pipe);
			
			return 0;
		}
};


#endif
