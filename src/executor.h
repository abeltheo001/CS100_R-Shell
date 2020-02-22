#include "../header/rshellclasses.h"
// #include "../header/expandAliases.h"
// #include "../header/checkBuiltin.h"

using namespace std;

int executor(CommandTree& ctree) { // Pass by reference
	Token* head = (ctree.getHead());
	if (head != nullptr) {
		head->execute();
		return (ctree.getHead())->status;
	} else {
		return 0;
	}
}
