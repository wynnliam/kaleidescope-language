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
	// TODO: Fix this! Will read "1.00.012..1412414" as a number.
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

	if(lastChar == '#') {
		do {
			lastChar = getchar();
		// Keep reading until we hit the end of the line.
		} while(lastChar != EOF && lastChar != '\n' && lastChar != '\r');

		// If we are not at the end of the file, start the process over again
		if(lastChar != EOF)
			return getToken();
	}

	return 0;
}
