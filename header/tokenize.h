#include "rshellclasses.h" // Includes vector and string
#include <unordered_set>

using namespace std;

vector<Token*> tokenize(vector<string> Vin) {
    // This function does the following:
    // While doing a single pass through the input vector string,
    //   store encountered words in a vector<string> buffer. When a
    //   recognized_operator is encountered, put the words in the buffer into
    //   a new Subcommand class.
    // Append the new Subcommand to grouped.
    // Make an Operator from the recognized operator and append it to grouped.
    // Repeat. 

    unordered_set<string> recognized_operators = { "||", "&&", ";" };
    vector<string> buffer;
    vector<shared_ptr<Token>> grouped;
    bool first = true;
    
    auto it = Vin.begin();
    while (it != Vin.end()) {
        if (recognized_operators.count(*it) == 1) {
            if (buffer.size() > 0) {
                auto subc = make_shared<Subcommand>(buffer);
                vector<string> oplist = {*it};
                auto op = make_shared<Operator>(oplist);
                // grouped.push_back(subc);
                // grouped.push_back(op);
            } else {
                // TODO: Implement error handling
            }
        }
        it++;
    }


}

