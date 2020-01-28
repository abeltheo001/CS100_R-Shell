# Design Document

### Project Information

Year: 2020

Quarter: Winter

Collaborators

Name: Syris Norelli

SID: 861312109

Name: Abel Theodoros

SID: 862021171

### Introduction

This project implements a shell, which allows for execution of programs from a command line (text input) interface. Commands (e.g. "echo hello") are mostly passed through to C++'s execvp() for execution, and the shell catches some special operators and commands. These will be as follows:

- Ordered/relative execution
    - Bash connectors ( "||", ";", "&&")
    - Parentheses (to be executed first, inner parentheses first)
- Aliases ( eg "i" → "sudo apt-get install" )
- Storage of state variables, like "pwd"
- Handling of comments ("echo hello #" prints hello )
- (Potentially) a config file, a la .bashrc or .vimrc

Parsing of statements like "echo a || echo b && echo c" is done by first filtering out comments and then constructing an "expression tree" (which in our implementation is of class CommandTree).

![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/9d6f4bd3-bdfa-47b5-9deb-e3fc3facc26f/CommandTree_(1).png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/9d6f4bd3-bdfa-47b5-9deb-e3fc3facc26f/CommandTree_(1).png)

In this data structure, the lowest level operations have the highest precedence. When there are no parentheses, the tree is formatted for "left precedence", which means that a command like "echo a || echo b && echo c" will run as follows:
"((echo a || echo b) && echo c)"

All nodes in this tree inherit from "Token," a class that stores a vector of strings. Token has subclasses "Subcommand" and "Operator", each with its own instructions on how the executor runs commands. The use of a vector<string> allows for differentiation of words in the Subcommand class.

Construction of the tree happens by converting the inorder input to preorder, and then constructing the tree node by node.

Execution consists of running a inorder traversal, wherein the left commands are executed first. When an Operator is reached, logic is used depending on the operator type on whether to execute the right Subcommand (eg || means "execute the right command only if a fails"). Success of the most recently executed Subcommand is passed up the tree so Operator logic works properly.

If a word in a Subcommand is in .rshell_aliases.txt, then it will be expanded to the defined command (in the same syntax as bash: alias i="sudo apt-get install"). This requires additional Tokenization, which is achieved by calling the "splitSpaces" and "tokenize" function from parser.

If the starting word in a Subcommand matches a special command name (e.g. exit), a call will be made to the relevant ~/src/name file rather than running execvp().

### Diagram

![OMT Diagram (for Assignment 1)](/images/omt_diagram.png?token=AKNKATP2GGHDH6PX2FKEORK6HEU2O)
### Classes

Create descriptions of each class and/or group of classes that you plan on developing. This can be as simple as a description of what each class accomplishes and how, or a pseudo-code level class definition. A class group would be a group of classes that all inherit from a single base class (composite classes are an example) and are therefore all closely related. For class, the group gives a description of the base class, as well as the differences between each child class. Make sure it is clear how these classes interact to perform the ingestion, transformations, and processing that is described at a high level in the introduction.

string userInput() : Outputs a "$" then takes in and stores the user's input as a string.

Commandtree parser(string) : Uses the user's string to form a vector of Tokens and then stored into a commandTree after having it parsed. 

- string filterComments(string): Takes in a string, identify if a "#" exists in it, and if so, return a substring that contains the string up until where the "#" previously existed.
- vector <string> splitSpaces(string): Takes in a string and form a vector of strings by going through the string and creating divisions whenever a space is found between text, inputting, although a separate case will later be made in what to do if the command requires two words to be in one string.
- vector<Token*> tokenize (vector<string>) : Takes in a vector of strings and convert it into a vector of Tokens. Iterate through the vector, store each value into a Token and push them into a new vector.
- Commandtree constructExTree(vector <Token*>): Take in a vector of Tokens and produce a command Tree, inserting the values in the vector in inorder.

int executor (Commandtree): Determines if there's been a failure, and if so, what kind of failure depending on the int returned. If the function returns a 0, there's been no error. 

- Subcommand expandAlias(Subcommand): This function is used if and only if the current Token is also a Subcommand, Looks into a table to aliases and sees if there's a match to the inputted Subcommand. If so, it will replace the command alias with the matching Subcommand and return it along with the rest of the command input.
- bool checkBuiltin(subCommand): Returns true if the subCommand has an intended location and if so, which function to use at that location.

### Prototypes/Research

Since you are likely unfamiliar with how the functions waitpid(), execvp() and fork() function individually and together you should create a small prototype function to test how these functions can be used together to execute small commands in a separate thread. In addition to a prototype for the main system functions, you should create a small prototype for parsing user input into the different elements necessary for completing assignment 2. The parser does not need to be bug-free or fully complete (although it must compile) but should represent your investigation into determining which parsing method you want to use for your assignment and the basics of a full parser (I suggest counting the number of times different types of elements appear in a given input as the output of your prototype). You should include the coded prototype you used to do your testing in the prototype/ directory and describe your findings and how you plan on using it in your assignment in this section (note these prototypes do not need an associated CMake file to compile them). Additionally, you will likely have questions about how connectors act in different situations. You should perform some preliminary testing of these different situations against the normal shell and write put any notes about the results in this section.

### Development and Testing Roadmap

1. Finish the design document
    1. Classes
    2. OMT Diagram
    3. Introduction polish
2. Convert design document to markdown format
3. Create prototypes for assignment 1
    1. waitpid()
    2. execvp()
    3. fork()
    4. a prototype parser that compiles (but does not necessarily complete all steps)
4. Prototypes writeup
5. After completion of the previous steps, check that compilation/testing works and clone it to a temporary directory for further work. This will ensure that the original directory compiles for the grader.

============== ASSIGNMENT 2 ==============

1. Create base classes
    1. Token
    2. Subcommand (inherits from Token)
    3. Operator (inherits from Token, can have Token* children)
    4. CommandTree (nodes are Token*)
2. Design AND TEST functions (any order):
    1. rshell
        1. string userInput()
    2. parser
        1. string filterComments(string)
        2. vector<string> splitSpaces(string)
        3. vector<Token*> tokenize(vector<string>)
        4. CommandTree constructExpressionTree(vector<Token*>)
    3. executor
        1. Subcommand expandAliases(Subcommand)
        2. bool checkBuiltin(Subcommand)
        3. int executeSubcommand(Subcommand)
3. Combine functions from previous step and make integration tests (any order):
    1. rshell
    string userInput()
    CommandTree parser(string)
    int executor(CommandTree)
    2. CommandTree parser(string)
    string filterComments(string)
    vector<string> splitSpaces(string)
    vector<Token*> tokenize(vector<string>)
    CommandTree constructExpressionTree(vector<Token*>)
    3. int executor(CommandTree)
    int executeInOrder(CommandTree)
        Subcommand expandAliases(Subcommand)
        bool checkBuiltin(Subcommand)
        int executeSubcommand(Subcommand)
4. Do a complete integration test (rshell → parser → executor)
