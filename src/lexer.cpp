#include "./lexer.h"
#include <cctype>

using namespace std;

// Filled in if the token we read is an identifier.
static string identifier;
// Filled in if the token is a number. All numbers are
// doubles in this language.
static double numVal;

int getToken() {
	static int lastChar = ' ';

	// Skip whitespace
	while(isspace(lastChar))
		lastChar = getchar();

	// Since we read and consume characters while
	// they are whitespace, at this point lastChar is not
	// whitespace.

	// Identifier regex is [a-zA-z][a-zA-Z0-9]*
	if(isalpha(lastChar)) {
		identifier = lastChar;

		while(isalnum((lastChar = getchar())))
			identifier += lastChar;

		if(identifier == "def")
			return tok_def;
		else if(identifier == "extern")
			return tok_extern;
	}

	// Digit regex is [0-9.]+
	if(isdigit(lastChar) || lastChar == '.') {
		string numberStr;

		do {
			numberStr += lastChar;
			lastChar = getchar();
		} while(isdigit(lastChar) || lastChar == '.');

		numVal = strtod(numberStr.c_str(), 0);
		return tok_number;
	}

	return 0;
}
