#ifndef RSHELLCLASSES_H
#define RSHELLCLASSES_H

#include <vector>
#include <string>

using namespace std;

class Token {
    public:
        Token() {};
        vector<string> getContent()          { return content; }
        void setContent(vector<string> V)    { content = V; }
        virtual int execute() = 0;
    protected:
        vector<string> content;
        Token* leftChild;
        Token* rightChild;
        int status = -1; // Current exit status of Token
};

class Subcommand : public Token {
    public:
        Subcommand(vector<string> V) { content = V; }
        int execute();
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
        CommandTree();
        void setHead(Token* t) { head = t; }
        Token* getHead() { return head; }
    protected:
        Token* head;
};

#endif