#ifndef RSHELLCLASSES_H
#define RSHELLCLASSES_H

#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <cassert>
#include <unordered_map>
#include <stdlib.h>
#include <sys/stat.h>
#include "rshellutils.h"
#include "executeCharArray.h"
#include "convertVectorToCharArray.h"

using namespace std;

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
        int status = -2; // Current exit status of Token. -2 is "hasn't run yet"
};

class Subcommand : public Token {
    public:
        virtual ~Subcommand() {}
        Subcommand(vector<string> V) { content = V; }
		bool operator==(Subcommand const rhs) const {
			return (this->content == rhs.content);
		}

        virtual int execute() {
 
		if (content[0] == "exit") {
			status = 0;
			exit(status);
		 }
		
		else if (content[0] == "test")	
		{
			if (test() == true)
			{ 
				cout << "(True)" << endl;
				status = 0;
			}
			else
			{ 
				cout << "(False)" << endl;
				status = 1;
			}
			
		} 
		else {

		char** chararr = convertVectorToCharArray(content);
		status = executeCharArray(chararr);
				
		if (status == -1) {
			cout << "RSHELL: Command not found!" << endl;
			}

		    for (int i = 0; i < content.size(); i++) {
			delete[] chararr[i];
		    }

		delete[] chararr;
		}

		return status;
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
};

class AndToken: public Token {
public: 
	    
	AndToken(vector<string> V) { content = V; }

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
		{ this->status = 1;}
            }
	    else {
  	     this->status = 1;}

	    return this->status; 
        }

};

class OrToken: public Token {
public: 
	OrToken(vector<string> V) { content = V; }

	bool operator==(OrToken const rhs) const {
			return (this->content == rhs.content);
		}

        virtual int execute() {
            int statusLeft, statusRight = -2;
            statusLeft = leftChild->execute();
            
            if (statusLeft != 0) {
                statusRight = rightChild->execute();
		if (statusRight == 0)
		{
			this->status = 0;
		}
		else 
		{ this->status = 1;}
            }
	    else {
  	     this->status = 1;}

	    return this->status; 
        }

};

class SemiToken: public Token {
public:
	SemiToken(vector<string V) {content = V; }
	
	bool operator==(SemiToken const rhs) const {
		return (this->content == rhs.content); }

	virtual int execute() { 
		int statusLeft, statusRight = -2;
		statusLeft = leftChild-> execute();

		if (content[0] == ";") {
			statusRight = rightChild->execute();
			if (statusRight == 0) {
			this->status = 0;
			}
		else 
		{ this->status = 1;}
            }
	else { this->status = 1;}
	
	return this->status;
	}
};
//Create AND, OR, SEMICOLOR TOKENS. 
//

class CommandTree {
    public:
        CommandTree() : head(nullptr) {}
        void setHead(Token* t) { head = t; }
        Token* getHead() { return head; }
	 	~CommandTree() {

            // std::cout << "In CommandTree::~CommandTree()" << std::endl;

            delChildren(head);
            // delete head;    // TODO: code this to delete children
            // head = nullptr;
			
			// Keeping this for now; am curious what might have been causing the issue.
		
			// // Delete nodes using DFS
            // if (head != nullptr) {
            //     std::queue<Token*> s;
            //     s.push(head);
            //     while (!(s.empty())) {
            //         Token* currNode = s.back();
            //         s.pop();
            //         //printVector(currNode->content);
            //         if (currNode->leftChild != nullptr) {
            //             s.push(currNode->leftChild);
            //         }
            //         if (currNode->rightChild != nullptr) {
            //             s.push(currNode->rightChild);
            //         }

            //         delete currNode;
            //     }
            // }
        }


        virtual string stringify() {
            // Initialize stack
            if (head != nullptr) {
            
                stack<pair<Token*,int>> s; // Stores token and number of spaces
                vector<string> output;
                
                pair<Token*, int> startelt;
                startelt.first = head;
                startelt.second = 0;
                s.push(startelt);

                bool lastPrint = 0; // Indicates whether the last thing printed was a leaf or not
                                    // 0: Last print was a Subcommand
                                    // 1: Last print was an Operator

                while (!(s.empty()) ) {
                    // Get top element
                    pair<Token*, int> topelt = s.top();
                    Token* curr = topelt.first;
                    int numSpaces = topelt.second;
                    s.pop();

                    string spaces(numSpaces, ' ');
					output.push_back(spaces);
					output.push_back(curr->stringify());
					output.push_back(" (");
					output.push_back(to_string(curr->status));
					output.push_back(")");
                    
					if (curr->hasChildren()) { // ie an Operator
                        output.push_back(" : {");
                        
                        // add right, then left so stack order prints properly

                        
                        if (curr->rightChild != nullptr) {
                            pair<Token*, int> add_to_stack;
                            add_to_stack.first = curr->rightChild;
                            add_to_stack.second = numSpaces + 2;
                            s.push(add_to_stack);
                        }

                        if (curr->leftChild != nullptr) {
                            pair<Token*, int> add_to_stack;
                            add_to_stack.first = curr->leftChild;
                            add_to_stack.second = numSpaces + 2;
                            s.push(add_to_stack);
                        }

                        lastPrint = 1;
                    } else {                // ie a Subcommand
                        if (lastPrint == 0) {
                            output.push_back("\n");
                            if (numSpaces >= 2) {
                                numSpaces -= 2;
                            }
                            string lessSpaces(numSpaces, ' ');
                            output.push_back(lessSpaces);
                            output.push_back("}");
                        }
                        lastPrint = 0;
                    }
                    output.push_back("\n");
                }
                return joinVector(output);
            }
            return "";
        }
		bool operator==(CommandTree &rct) {
            // Method: Use DFS to compare each node in place in trees.
            // Set up stacks
            stack<Token*> lefts;
            stack<Token*> rights;
            lefts.push(head);
            rights.push(rct.getHead());

            // DFS
            while (!(lefts.empty() || rights.empty())) {
                Token* currLeft = lefts.top();
                Token* currRight = rights.top();
                lefts.pop();
                rights.pop();

                if (!(*currLeft == *currRight)) {
                    cout << "Failed on " 
                         << endl 
                         << currLeft->stringify() 
                         << endl 
                         << currRight->stringify() 
                         << endl;
                    return false;
                }

                if (currLeft->leftChild != nullptr) {
                    lefts.push(currLeft->leftChild);
                }
                if (currLeft->rightChild != nullptr) {
                    lefts.push(currLeft->rightChild);
                }

                if (currRight->leftChild != nullptr) {
                    rights.push(currRight->leftChild);
                }
                if (currRight->leftChild != nullptr) {
                    rights.push(currRight->rightChild);
                }
            }

            // Trees should be of same size
            if (!(lefts.empty() && rights.empty())) {
                cout << "Failed when trees were not same size." << endl;
                return false;
            }

            return true;
		}
	private:
        // preorder, bc why not
        void delChildren(Token*& root) {

            // BC: root is nullptr
            if (root == nullptr) return;

            // BC: root has no children
            if (!root->hasChildren()) {
                delete root;
                root = nullptr;
                return;
            }

            // Else, call this on root's left and root's right
            if (root->leftChild != nullptr)
                delChildren(root->leftChild);
            if (root->rightChild != nullptr)
                delChildren(root->rightChild);

            // With all of root's children gone, delete it and return
            delete root;
            root = nullptr;

            return;

        }

    protected:
        Token* head;
};

class RShell {
    public:
		RShell() {}
		RShell(string filename) {
			// TODO: Make this take in .rshellrc and set up configData accordingly
		}
		virtual ~RShell() {
			if (currentTree != nullptr) {
				delete currentTree;
			}
		}

		void makeCommandTree(string);
		int executeCommandTree();
		
		//Member variables;
		CommandTree* currentTree = nullptr;
		unordered_map <string, string> configData;
		bool DEBUG = true;
   
  	private: 
		bool checkBuiltin(vector<string>);
		void constructSubTree(const vector<Token*>&, int);
		vector<string> groupQuotes(vector<string>);
		vector <string> filterComments (vector <string>);
		vector<Token*> tokenize(vector<string>);
		void constructExpressionTree(vector<string>);
};
#endif
