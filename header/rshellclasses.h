#ifndef RSHELLCLASSES_H
#define RSHELLCLASSES_H

#include <vector>
#include <string>
#include <stack>
#include "rshellutils.h"
#include "executeVector.h"

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
        string stringify() { return joinVector(content, ' '); }

        // Member variables
        vector<string> content;
        Token* leftChild = nullptr;
        Token* rightChild = nullptr;
        int status = -2; // Current exit status of Token. -2 is "hasn't run yet"
};

class Subcommand : public Token {
    public:
        Subcommand(vector<string> V) { content = V; }
		bool operator==(Subcommand const rhs) const {
			return (this->content == rhs.content);
		}
        virtual int execute() { 
            if (content[0] == "exit") {
                exit(0);
            }
            status = executeVector(content);
			return status;
		}
};

class Operator : public Token {
    public:
        Operator(vector<string> V) { content = V; }
		bool operator==(Operator const rhs) const {
			return (this->content == rhs.content);
		}
        void makeStatus(int a, int b) {
            if (b == -2) { // The right subcommand didn't run 
                // Either:
                // a succeeded in a || b (overall success)
                // a failed in    a && b (overall failure)
                if (a == 0) {
                    this->status = 0;
                } else {
                    this->status = 1;
                }
            } else { // Both Subcommands ran
                // Either:
                // a failed and b ? in  a || b (pass b up)
                // a succeed and b ? in a && b (pass b up)
                // a ? and b ? in       a ;  b (pass b up)
                // All cases mean passing b success up, so just return 0 if b=0, 1 otherwise.
                if (b == 0) {
                    this->status = 0;
                } else {
                    this->status = 1;
                }
            }
        }
        virtual int execute() {
            int statusLeft, statusRight = -2;
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
        void setHead(Token* t) { head = t; }
        Token* getHead() { return head; }
		~CommandTree() {
            // Delete nodes using BFS
            stack<Token*> s;
            s.push(head);
            while (!(s.empty())) {
                Token* currNode = s.top();
                s.pop();
                if (currNode->leftChild != nullptr) {
                    s.push(currNode->leftChild);
                }
                if (currNode->rightChild != nullptr) {
                    s.push(currNode->rightChild);
                }
                delete currNode;
            }
        }
        string stringify() {
            // Initialize stack
            stack<pair<Token*,int>> s; // Stores token and number of spaces
            vector<string> output;
            pair<Token*, int> startelt;
            startelt.first = head;
            startelt.second = 0;
            s.push(startelt);

            bool lastPrint = 0; // Indicates whether the last thing printed was a leaf or not
                                // 0: Last print was a  Subcommand
                                // 1: Last print was an Operator

            while (!(s.empty()) ) {
                // Get top element
                pair<Token*, int> topelt = s.top();
                Token* curr = topelt.first;
                int numSpaces = topelt.second;
                s.pop();

                string spaces(numSpaces, ' ');
                if (curr->hasChildren()) {
                    output.push_back(spaces);
                    output.push_back(curr->stringify());
                    output.push_back(" (");
                    output.push_back(to_string(curr->status));
                    output.push_back(")");
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
                } else {
                    output.push_back(spaces);
                    output.push_back(curr->stringify());
                    output.push_back(" (");
                    output.push_back(to_string(curr->status));
                    output.push_back(")");
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
    protected:
        Token* head;
};

#endif
