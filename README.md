# CS100 R'Shell Project

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

![Command Tree](/images/CommandTree.png)

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

Base class: Token
- Stores word data as parts of commands.
- Class variables
    - vector<string> content (stores words)
- Class functions
    - Token()
    - Token(vector<string>)
    - vector<string> getContent()
    - void setContent(vector<string>)

Inherited class: Subcommand : Token
- Stores actual subcommand data. For example, {"echo", "a"}.
- Class variables
    - [None additional]
- Class functions
    - Subcommand(vector<string>)

Inherited class: Operator : Token
- Meant for storage of connectors like ||, &&, and ;. Used as a signal to the executor to keep previous success information available for connector logic. 
- Class variables
    - vector<Token*> children (No Operators should be leaf nodes of CommandTree, and Subcommands should always be leaf nodes, so only the Operator Token stores a vector<Token*> children class variable.)
- Class functions
    - Operator(vector<string>)

Base class: CommandTree
- CommandTree stores a Token* head of the execution tree. The executor uses inorder traversal to execute commands on the tree, and stops going down the tree as soon as a Subcommand* is encountered.
- Class variables
    - Token* head
- Class functions
    - CommandTree()
    - void setHead(Token*)
    - Token* getHead()

Our classes don't directly interact very often, so we decided to take this space to describe our intended functions as well.

rshell (executable main)

- string userInput() : Outputs a "$" then takes in and stores the user's input as a string.

CommandTree parser(string) : Uses the user's string to form a vector of Tokens which is then converted into a CommandTree. 

- vector<string> splitSpaces(string): Takes in a string and forms a vector of strings by going through the string and creating divisions whenever a space is found between text. This later needs to be modified to deal with quotes, which will be achieved by first separating out the quotes and then splitting each non-quote remainder string. The intermediate steps would look something like this: {"echo a || echo", "hello world"} - > {"echo", "a", "||", "echo", "hello world"}
- vector<string> filterComments(vector<string>): Takes in the vector from the last step, identifies if a "#" exists in it, and returns a vector of all terms before the "#".
- vector<Token*> tokenize (vector<string>) : Takes in a vector of strings and convert it into a vector of Tokens. Iterate through the vector, store each value into a Token child class and push them into a new vector. This function does detection of Operators (||, &&, ;) and Subcommands and tags them as such during object initialization.
- CommandTree constructExTree(vector<Token*>): Take in a vector of Token*s and produce a CommandTree. This is achieved through conversion of the vector from inorder to preorder, which makes constructing a tree in a top down manner much easier.

int executor (CommandTree): Determines if there's been a failure, and if so, what kind of failure depending on the int returned. If the function returns a 0, there's been no error. 

- Subcommand expandAlias(Subcommand): This function is used if and only if the current Token is also a Subcommand. expandAlias checks if each of the words in the command are loaded into an unordered_set of aliases loaded at program start time. If there's a word match, it will replace the word with the matching group of words. For example, the alias file might read:
alias i="sudo apt-get install"
So if the Subcommand content was {"i", "cowsay", "&&", "cowsay", "hello world"}, then the expanded version would read:
{"sudo", "apt-get", "install", "cowsay", "&&", "cowsay", "hello world"}
- bool checkBuiltin(Subcommand): Returns true if the first word of the Subcommand matches a rshell function (eg exit).

### Prototypes/Research

For error-tolerant execution of commands, the safest way is probably to fork a process and run execvp() in that fork. This allows for extraction of errors - if the child process fails, the parent process can monitor the return state of the child. The functionality of waitpid() and fork() are demonstrated in ~/prototype/prototype_waitpid.cpp and the functionality of execvp() is demonstrated in ~/prototype/prototype_execvp.cpp.

For our parser, early functionality can be found in ~/src/parser.cpp, with the imported header files demonstrating functionality. Both ~/header/splitSpaces.h and ~/header/filterComments.h have tests in ~/unit_tests/test.cpp. 

Early on, we were hoping that vector<Token*> would store inherited children of Token* (Subcommand*, Operator*) but apparently C++ dislikes doing this without special effort. In our assignment two, we will research using smart pointers to handle this functionality. If this does not work, our base classes will be modified. As a result, the tokenize function is not complete yet.

For bash style connectors ( ||, &&, ; ), we found the following result in bash:

$ echo a || echo b && echo c

a

c

Ths implies "left precedence" which is essentially a reduce function that works from left to right. First "echo a || echo b" is executed. Since echo a succeeds, echo b is not run. As a group, "echo a || echo b" succeeded at least once, so "echo c" runs.

This is straightforward to implement as it only requires the addition of an accumulator variable (in this case, a bool that stores whether the last command succeeded), but due to concerns about later extensibility, we decided to go with a tree structure instead for storing the command logic. For example, if there are parentheses, then the left to right ordering no longer applies.

### Development and Testing Roadmap

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
