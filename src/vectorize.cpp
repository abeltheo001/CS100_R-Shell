#include "vectorize.h"

int main(int argc, char** argv) {
	vector<string> v = {};
	vectorize(argc, argv, v);
	printVector(v);
	return 0;
}
