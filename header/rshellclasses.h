#ifndef RSHELLCLASSES_H
#define RSHELLCLASSES_H

#include <vector>
#include <string>
#include <stack>

using namespace std;

class Token {
    public:
        Token() {};

        vector<string> getContent()          { return content; }
        void setContent(vector<string> V)    { content = V; }

        virtual int execute() = 0;

        void setLeft(Token* t) { leftChild = t; }
        void setRight(Token* t) { rightChild = t; }
        bool hasChildren() { return ((leftChild != nullptr) || (rightChild != nullptr)); }
		bool operator==(Token const &rhs) const {
			return (this->content == rhs.content);
		}

        vector<string> content;
        Token* leftChild;
        Token* rightChild;
        int status = -1; // Current exit status of Token
};

class Subcommand : public Token {
    public:
        Subcommand(vector<string> V) { content = V; }
        int execute() { // TODO: To be implemented
		return 0;
	}
};

class Operator : public Token {
    public:
        Operator(vector<string> V) { content = V; }
        void makeStatus(int a, int b) {
            if (b == -1) {
                if (a == 0) {
                    this->status = 0;
                } else {
                    this->status = 1;
                }
            } else {
                if ((a == 0) || (b == 0)) {
                    this->status = 0;
                } else {
                    this->status = 1;
                }
            }
        }
        int execute() {
            int statusLeft, statusRight = -1;
            statusLeft = leftChild->execute();
            
            // Always run ;
            if (content[0] == ";") {
                statusRight = rightChild->execute();
            }
            
            // Conditionally run && and ||
            if ((statusLeft == 0) && (content[0] == "&&")) {
                statusRight = rightChild->execute();
            }
            if ((statusLeft != 0) && (content[0] == "||")) {
                statusRight = rightChild->execute();
            }

            makeStatus(statusLeft, statusRight);
        }
};

class CommandTree {
    public:
        CommandTree() : head(nullptr) {}
        ~CommandTree() {
            // Delete nodes using BFS
            stack s;
            s.push_back(head);
            while (!(s.empty())) {
                Token* currNode = s.top();
                s.pop();
                if (currNode->leftChild != nullptr) {
                    s.push_back(currNode->leftChild);
                }
                if (currNode->rightChild != nullptr) {
                    s.push_back(currNode->rightChild);
                }
                delete currNode;
            }
        }

        void setHead(Token* t) { head = t; }
        Token* getHead() { return head; }
        
    protected:
        Token* head;
};

#endif
