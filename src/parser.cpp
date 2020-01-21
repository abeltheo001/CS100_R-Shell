#include <iostream>

using namespace std;

class Token {
	public:
		Token(string);
	private:
		string data;
}

class MultiToken {
	public:
		MultiToken(vector<Token>&)
	private:
		vector<Token>& words;
}

MultiToken* parse(string) {
;
}
