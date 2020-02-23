#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "rshellclasses.h"
#include "rshellutils"
#include "rshellclasses.h"
#include "splitOnChar.h"
#include "groupQuotes.h"
#include "filterComments.h"
#include "tokenize.h"
#include "constructExpressionTree.h"

using namespace std;

/*
 * Rshell::Rshell(string input)
	{//Push into command Tree}

  virtual Rshell::~Rshell() {}
*/

void Rshell::makeCommandTree(string input)
{
	vector<string> test = splitOnChar(userInput,';');
	userInput = joinVector(test," ;"); 

	// 1. Split on spaces
		vector<string> words = splitOnChar(userInput, ' ');
 	// 2. Make sure quotes get grouped together
		words = groupQuotes(words);

	// 3. Filter out 					
		words = filterComments(words);

	// 4. Group words/quotes into Tokens of type Subcommand or Operator.
 		vector<Token*> tokens = tokenize(words);

	// 5. Create a tree for execution in later steps
		currentTree = constructExpressionTree(tokens);
}

int Rshell::executeCommand(CommandTree& currentTree)
{
	Token* head = currentTree.getHead();
	if (head != nullptr) {
		head->execute();
		return (currentTree.getHead())->status;
	} else {
		return 0;
	}
	
}


bool Rshell::checkBuiltin(vector <string> input)
{
	unordered_set<string> operations = {"exit"};
	return (operations.count(input[0]) > 0);
}

void Rshell::constructSubTree(const vector<Token*>& allNodes , int currPos)
{
    // Constructs a three subgroup, then recurses
    //     //         ...
    //         //       B
    //             //     A   C
    //                 // ...  ...
    //                     // currPos refers to C in the three subgroup
	if (currPos == 2) 
	{ 
		allNodes[currPos-1]->rightChild = allNodes[currPos];
		allNodes[currPos-1]->leftChild = allNodes[currPos-2];
		return;	
	}
	else
	{
	// allNodes[currPos-1] is the operator
		allNodes[currPos-1]->rightChild = allNodes[currPos];
		allNodes[currPos-1]->leftChild = allNodes[currPos-3];
		constructSubTree(allNodes, currPos-2);
	}
}

vector<string> Rshell::groupQuotes(vector<string> Vin)
{
	vector<string> grouped;
	vector<string> buffer;	
	bool state = 0;

	auto it = Vin.begin();
	while (it != Vin.end()) {
		bool quotefound = false;
		for (char c : (*it)) 
		{ 
			if (c == '"') 
			{
				quotefound = true;
				break;
			}
		}
		if (quotefound) 
		{
			if (state) 
			{ 
				state = 0;
				buffer.push_back(*it);
				string concat = joinVector(buffer, ' ');
				grouped.push_back(concat);
				buffer.clear();
			} 

			else 
			{ 
				state = 1;
				buffer.push_back(*it);
			}
		} 
		else 
		{
			if (state) 
			{ 
				buffer.push_back(*it);
			} 
			else 
			{ 
				grouped.push_back(*it);
			}
		}
		it++;
	}

	if (buffer.size() > 0) 
	{
        	string s = "No end quote in quote grouping.";
        	const char* errormsg = s.c_str();
        	perror(errormsg);
	}

	return grouped;

}

vector<string> Rshell::filterComments (vector <string> input)
{
	vector <string> result;
        for (vector<string>::iterator it = input.begin(); it != input.end(); ++it)
            {
		        string check = *it;
		    if (check[0] == '#')
		    {
			    break;
		    }
		    else
		    {
			    result.push_back(check);
		    }
        }

        return result; 
}

vector<Token*> Rshell::tokenize(vector<string> Vin)
{
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
                string s = "No end quote in quote grouping.";
                const char* errormsg = s.c_str();
                perror(errormsg);
            }
        } else {
            buffer.push_back(*it);
        }
        it++;
    }

    if (buffer.size() > 0) {
		Subcommand* subc = new Subcommand(buffer);
		grouped.push_back(subc);
    }

    return grouped;
}
