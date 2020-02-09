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
    vector<Token*> grouped;
    bool first = true;
    
    auto it = Vin.begin();
    while (it != Vin.end()) {
        if (recognized_operators.count(*it) > 0) {
            if (buffer.size() > 0) {
                Subcommand* subc = new Subcommand(buffer);
                vector<string> oplist = {*it};
                Operator* op = new Operator(oplist);
                
				grouped.push_back(subc);
                grouped.push_back(op);
            
				buffer.clear();
	    	} else {
                // TODO: Implement error handling
            }
        } else {
            buffer.push_back(*it);
        }
        it++;
    }

    // Remove remaining on buffer
    if (buffer.size() > 0) {
		Subcommand* subc = new Subcommand(buffer);
		grouped.push_back(subc);
    }

    return grouped;
}

