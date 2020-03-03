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

This project implements a shell, which allows for execution of programs from a command line (text input) interface. Commands (e.g. "echo hello") are mostly passed through to C++'s execvp() for execution, and the shell catches some special operators and commands. These are as follows:

- Ordered/relative execution
    - Bash connectors ( "||", ";", "&&")
    - Parentheses (inner parentheses are executed first)
- Handling of comments ("echo hello #" prints hello )
- "exit" causes RShell to clean up its own memory and exit
- test command
	- "test" works similarly to bash - allows for -e, -f, -d flags
	- symbolic test calls: "[ -e /path/to/file ]" is handled
- quotes are allowed - eg "echo " ##### "" doesn't stop at #

Some planned special constructs to catch:

- Aliases ( eg "i" → "sudo apt-get install" )
- Storage of state variables, like "pwd"
- Construction of the RShell with a config file, a la .bashrc or .vimrc

Parsing of statements happens in header/shuntingYardConstruct.h, which directly constructs the postfix queue from the input string. The algorithm is based around Shunting Yard, and roughly follows these steps:

for char c in inString

	if c and previous chars are equivalent to "&&", "||", or ";". This means we're currently on an operator.
		Flush the buffer (except for the length of the operator) and store it in a Subcommand.
		Flush the operator to its operator type, such as AndToken, OrToken, or SemiToken.
		Handle the Subcommand and operator Token in the same manner as Shunting Yard. In particular,
			Subcommand gets sent to the outputQueue, and the operator Token gets sent to the operator stack.
			If there are any operators on the operator stack, push those to the outputQueue before pushing onto the stack.
	else if c is ( or [ or ",
		then a block of inString needs to grouped and dealt with.
		first, find the relevant "close" position for this char. If c = (, then close = ).
		this skips nested parentheses and only outputs the depth when the close paren is at the same depth.
		if parentheses
			recursively run shuntingYardConstruct and place the outputQueue into a ParenthesisToken. This is for proper handling of precedence order.
		if brackets
			set the inside to a TestToken
		if "
			add the quote block to the buffer
	else
		store c in a buffer of strings

if buffer is nonempty
	flush buffer to a Subcommand and push it onto outputQueue.

Execution is comparatively more straightforward and follows the same pattern as the Shunting Yard algorithm:

for Token\* t in outputQueue
	if not operator
		push to execution stack
	else
		pop top two elements from execution stack
		set first and second elements to operator right and left children (respectively)
		retval = operator->execute()
		delete any leftover StorageTokens from previous steps
		create a new StorageToken(retval) so future operators have access to the return value and push it to the execution stack

### Why are parentheses treated recursively?

Consider the following command:

echo a || (false && echo c)

When tokenized inorder, this command can be represented as:

{Subcommand(echo a), OrToken(||), Subcommand(false), AndToken(&&), Subcommand(echo c)}

Canonical Shunting Yard would produce this as the queue output:

{Subcommand(echo a), Subcommand(false), Subcommand(echo c), AndToken(&&), OrToken(||)}

While this would be correct for parentheses if we were doing algebra, we don't want the right to execute at all when doing bash connectors properly.

However, if we wrap the parenthesis group into a ParenthesisToken, then execution works properly. Here is the inorder form:

{Subcommand(echo a), OrToken(||), ParenthesisToken({Subcommand(false), AndToken(&&), Subcommand(echo c)})}

After Shunting Yard:

{Subcommand(echo a), ParenthesisToken(...), OrToken(||)}

which produces the expected behavior.

### Diagram

![OMT Diagram (for Assignment 2)](/images/omt_diagram_2.png)

### Classes

Class: RShell
- Keeps track of the runtime of the entire shell. Stores config values and keeps track of memory.
- Class variables
	- deque\<Token\*\> commandDeque
	- bool DEBUG
	- unordered\_map\<string, string\> configData
- Class functions
	- RShell()
	- RShell(bool) (set DEBUG value)
	- RShell(string) (for future config setup)
	- virtual ~RShell()
	- void deconstructDeque()
	- void makeCommandDeque(string)
	- int executeCommandDeque()
	- deque<Token\*> shuntingYardConstruct(string)
	- int shuntingExecute(deque<Token\*>)
	- int findClose(const string&, int, char)

Base class: Token
- Stores word data as parts of commands.
- Class variables
    - vector<string> content (stores words)
	- Token\* leftChild
	- Token\* rightChild
	- int status (stores the current execution status of a command)
	- bool isOperator (used during Shunting Yard execute)
- Class functions
    - Token()
    - Token(vector<string>)
	- virtual string stringify();
	- virtual int execute() = 0
	- bool hasChildren()
	- bool operator==(Token const &rhs) const

Inherited class: Subcommand : Token
- Stores actual subcommand data. For example, {"echo", "a"}.
- Class variables
    - [None additional]
- Class functions
    - Subcommand(vector<string>)
	- virtual int execute()
	- virtual string stringify()
	- bool operator==(Subcommand const rhs) const
	- bool test() (used for the execution of the test command)

Inherited class: AndToken : Token
- Represents && as a Token. Its execute() deals with the conditional execution logic needed for &&.
- Class variables
	- [None additional]
- Class functions
    - AndToken(vector<string>)
	- virtual int execute()
	- virtual string stringify()
	- bool operator==(AndToken const rhs) const

Inherited class: OrToken : Token
- Represents || as a Token. Its execute() deals with the conditional execution logic needed for ||.
- Class variables
	- [None additional]
- Class functions
    - OrToken(vector<string>)
	- virtual int execute()
	- virtual string stringify()
	- bool operator==(OrToken const rhs) const

Inherited class: SemiToken : Token
- Represents ; as a Token. Its execute() deals with the conditional execution logic needed for ;.
- Class variables
	- [None additional]
- Class functions
    - SemiToken(vector<string>)
	- virtual int execute()
	- virtual string stringify()
	- bool operator==(SemiToken const rhs) const

Inherited class: ParenthesisToken : Token
- Essentially acts as a decorator around a commandDeque. When told to execute, it runs shuntingExecute on itself.
- Class variables
	- deque<Token\*> interior
- Class functions
    - ParenthesisToken(deque<Token\*>)
	- virtual int execute()
	- virtual string stringify()

Inherited class: TestToken : Token
- Holds the internals of [ -e /path/to/file ]. Functionally equivalent to a Subcommand token without the word "test" in front of its this-\>content.
- Class variables
	- [None additional]
- Class functions
    - TestToken(deque<Token\*>)
	- virtual int execute()
	- virtual string stringify()

Inherited class: StorageToken : Token
- Exists as temporary storage for a status during shuntingExecute.
- Class variables
	- [None additional]
- Class functions
    - StorageToken(int)
	- virtual int execute()
	- virtual string stringify()

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

### Prototypes/Research

For error-tolerant execution of commands, the safest way is probably to fork a process and run execvp() in that fork. This allows for extraction of errors - if the child process fails, the parent process can monitor the return state of the child. The functionality of waitpid() and fork() are demonstrated in ~/prototype/prototype_waitpid.cpp and the functionality of execvp() is demonstrated in ~/prototype/prototype_execvp.cpp.

For our parser, early functionality can be found in ~/src/parser.cpp, with the imported header files demonstrating functionality. Both ~/header/splitSpaces.h and ~/header/filterComments.h have tests in ~/unit_tests/test.cpp. 

For details on our Shunting Yard functionality, see the Introduction.

### Development and Testing Roadmap

[None left until assignment 4.]

