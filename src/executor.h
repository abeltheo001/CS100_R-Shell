#include "../header/rshellclasses.h"
#include "../header/expandAliases.h"
#include "../header/checkBuiltin.h"
#include "../header/executeVectorString.h"

using namespace std;

int executor(CommandTree ctree) {
	(ctree.getHead())->execute();
	return (ctree.getHead()).status
}
