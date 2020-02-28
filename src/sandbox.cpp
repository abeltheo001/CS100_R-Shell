#include <iostream>
#include "parser.h"
#include "executor.h"

using namespace std;

void testLeaf() {

    vector<string> left = {"echo", "hello", "world"};

    CommandTree ct;
    ct.setHead(new Subcommand(left)); // gets deleted in ct's dtor

    int retval = executor(ct);
}

void testConnector() {

    vector<string> left_v = {"echo", "Pog"};
    vector<string> right_v = { "echo", "Champ" };
    vector<string> op_v = { "||" };

    Token* op = new Operator(op_v);
    op->leftChild = new Subcommand(left_v);
    op->rightChild = new Subcommand(right_v);

    CommandTree ct;
    ct.setHead(op);

    executor(ct);

}

int main() {

    // testLeaf(); // works, no mem leak

    testConnector();

    return 0;
}
