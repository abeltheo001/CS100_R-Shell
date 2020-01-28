#ifndef RSHELLCLASSES_H
#define RSHELLCLASSES_H

#include <vector>
#include <string>
#include <memory> // Needed for many Token* tree stuff; use shared_ptr

class Token {
    public:
        Token();
        Token(vector<string> V) : content(V) {};
        vector<string> getContent()          { return content; };
        void setContent(vector<string> V)    { content = V; };
    protected:
        vector<string> content;
};

class Subcommand : Token {
    public:
        Subcommand(vector<string> V) { content = V; };
};

class Operator : Token {
    public:
        Operator(vector<string> V) { content = V; };
    protected:
        vector<Token*> children;
};

class CommandTree {
    public:
        CommandTree();
        void setHead(Token* t) { head = t; };
        Token* getHead() { return head; };
    protected:
        Token* head;
};

#endif