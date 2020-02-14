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

All nodes in this tree inherit from "Token," a class that stores a vector of strings. Token has subclasses "Subcommand" and "Operator", each with its own composite-patterned execute() function. The use of a vector<string> allows for differentiation of words in the Subcommand class.

Construction of the tree (at the moment) happens by simple backward recursion from the right of the command. This will break when parentheses are added, so it is only used as a stopgap measure. Later, we hope to converting the inorder command input to preorder, and then constructing the tree from the top down using that information.

Execution consists of calling execute() on Commandtree's head. As this follows the strategy pattern design paradigm, each Operator passes down the execute() call to the Subcommands and handles conditional execution based on the return values.

[TODO] If a word in a Subcommand is in .rshell_aliases.txt, then it will be expanded to the defined command (in the same syntax as bash: alias i="sudo apt-get install"). 

If the starting word in a Subcommand matches a special command name (e.g. cd), a call will be made to the relevant ~/src/name file rather than running execvp(). If "exit" shows up in a Subcommand, exit(0) is called.

### Diagram

[OMT Diagram (for Assignment 1)](/images/omt_diagram_2.png)
### Classes

Base class: Token
- Stores word data as parts of commands.
- Class variables
    - vector<string> content (stores words)
	- Token\* leftChild
	- Token\* rightChild
	- int status (stores the current execution status of a command)
- Class functions
    - Token()
    - Token(vector<string>)
    - vector<string> getContent() (defunct; either switch to proper encapsulation or remove get/setters)
    - void setContent(vector<string>) (defunct; either switch to proper encapsulation or remove get/setters)
	- void setLeft(Token\*) (defunct; either switch to proper encapsulation or remove get/setters)
	- void setRight(Token\*) (defunct; either switch to proper encapsulation or remove get/setters)
	- bool hasChildren()
	- bool operator==(Token const &rhs) const
	- string stringify() 
	- virtual int execute() = 0

Inherited class: Subcommand : Token
- Stores actual subcommand data. For example, {"echo", "a"}.
- Class variables
    - [None additional]
- Class functions
    - Subcommand(vector<string>)
	- int execute()

Inherited class: Operator : Token
- Meant for storage of connectors like ||, &&, and ;. Its execute() function handles connector logic.
- Class variables
	- [None additional]
- Class functions
    - Operator(vector<string>)
	- void makeStatus(int, int) (figures out whether the compound statement "succeeded". for example, "false || echo b" counts as a success because the right side succeeded.)
	- int execute()

Base class: CommandTree
- CommandTree stores a Token\* head of the execution tree.
- Class variables
    - Token\* head
- Class functions
    - CommandTree()
    - void setHead(Token\*) (defunct; either switch to proper encapsulation or remove get/setters)
    - Token\* getHead() (defunct; either switch to proper encapsulation or remove get/setters)
	- string stringify() (Outputs command tree in a nicely readable format)
	- bool operator==(CommandTree&) (uses DFS to check if all nodes are equal, and if the trees are of the same size)

Our classes don't directly interact very often, so we decided to take this space to describe our functions as well.

rshell (executable main)

CommandTree parser(string) : Forms a vector of Tokens from input string which is then converted into a CommandTree. 

- vector<string> splitOnChar(string, char): Takes in a string and forms a vector of strings by going through the string and creating divisions whenever an input char (eg space) is found between text. 
- vector<string> filterComments(vector<string>): Takes in the vector from the last step, identifies if a "#" exists in it, and returns a vector of all terms before the "#".
- vector<Token\*> tokenize (vector<string>) : Takes in a vector of strings and convert it into a vector of Tokens. Iterate through the vector, store each value into a Token child class and push them into a new vector. This function does detection of Operators (||, &&, ;) and Subcommands and tags them as such during object initialization.
- CommandTree constructExpressionTree(vector<Token\*>): Take in a vector of Token\*s and produce a CommandTree.

int executor (CommandTree): Determines if there's been a failure, and if so, what kind of failure depending on the int returned. If the function returns a 0, there's been no error. 

- Subcommand expandAlias(Subcommand): This function is used if and only if the current Token is also a Subcommand. expandAlias checks if each of the words in the command are loaded into an unordered_set of aliases loaded at program start time. If there's a word match, it will replace the word with the matching group of words. For example, the alias file might read:
alias i="sudo apt-get install"
So if the Subcommand content was {"i", "cowsay", "&&", "cowsay", "hello world"}, then the expanded version would read:
{"sudo", "apt-get", "install", "cowsay", "&&", "cowsay", "hello world"}
- bool checkBuiltin(Subcommand): Returns true if the first word of the Subcommand matches a rshell function (eg cd).

### Prototypes/Research

For error-tolerant execution of commands, the safest way is probably to fork a process and run execvp() in that fork. This allows for extraction of errors - if the child process fails, the parent process can monitor the return state of the child. The functionality of waitpid() and fork() are demonstrated in ~/prototype/prototype_waitpid.cpp and the functionality of execvp() is demonstrated in ~/prototype/prototype_execvp.cpp.

For our parser, early functionality can be found in ~/src/parser.cpp, with the imported header files demonstrating functionality. Both ~/header/splitSpaces.h and ~/header/filterComments.h have tests in ~/unit_tests/test.cpp. 

Early on, we were hoping that vector<Token\*> would store inherited children of Token\* (Subcommand\*, Operator\*) but apparently C++ dislikes doing this without special effort. In our assignment two, we will research using smart pointers to handle this functionality. If this does not work, our base classes will be modified. As a result, the tokenize function is not complete yet.

For bash style connectors ( ||, &&, ; ), we found the following result in bash:

$ echo a || echo b && echo c

a

c

Ths implies "left precedence" which is essentially a reduce function that works from left to right. First "echo a || echo b" is executed. Since echo a succeeds, echo b is not run. As a group, "echo a || echo b" succeeded at least once, so "echo c" runs.

This is straightforward to implement as it only requires the addition of an accumulator variable (in this case, a bool that stores whether the last command succeeded), but due to concerns about later extensibility, we decided to go with a tree structure instead for storing the command logic. For example, if there are parentheses, then the left to right ordering no longer applies.

### Development and Testing Roadmap

============== PRE ASSIGNMENT 3 ==============

1. [Rewrite rshell to be a class and passed as a reference to underlying functions. This will allow for .rshellrc references globally, and is a general a more flexible structure.](../../issues/57)
2. Add additional functionality
	1. [expandAliases](../../issues/25)
	2. $ color
	3. other .rshellrc features
3. [Work on adding parentheses parsing to the parse function. This pre-empts assignment 3.](../../issues/65)
