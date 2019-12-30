#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <cctype>
#include <iostream>

using namespace std;

// When we get a bad token, we assign it a non-negative
// number. I know this is somewhat backwards, but this
// works very well for enums.
enum Token {
	tok_eof = -1,

	// Commands. def is for functions we define inside
	// a kaleidescope file, and extern for those defined
	// in the standard library.
	tok_def = -2,
	tok_extern = -3,

	// primary token types
	tok_identifier = -4,
	tok_number = -5,
};

class ExpressionAbstractSyntaxTree {
	public:
		virtual ~ExpressionAbstractSyntaxTree();
};

class NumberExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		NumberExpressionAbstractSyntaxTree(const double val) : value(val) { }

	private:
		double value;
};

class VariableExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		VariableExpressionAbstractSyntaxTree(const std::string& name) : name(name) { }
	
	private:
		std::string name;
};

class BinaryExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		BinaryExpressionAbstractSyntaxTree(const char op,
										   std::unique_ptr<ExpressionAbstractSyntaxTree> lhs,
										   std::unique_ptr<ExpressionAbstractSyntaxTree> rhs) :
										   op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) { }

	private:
		char op;
		std::unique_ptr<ExpressionAbstractSyntaxTree> lhs, rhs;
};

class CallExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		CallExpressionAbstractSyntaxTree(const std::string& callee,
										 std::vector<std::unique_ptr<ExpressionAbstractSyntaxTree>> args) :
										 callee(callee), args(std::move(args)) { }
	private:
		std::string callee;
		std::vector<std::unique_ptr<ExpressionAbstractSyntaxTree>> args;
};

class PrototypeExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		PrototypeExpressionAbstractSyntaxTree(const std::string& name, std::vector<std::string> args) :
											  name(name), args(std::move(args)) { }

	private:
		std::string name;
		std::vector<std::string> args;
};

class FunctionExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		FunctionExpressionAbstractSyntaxTree(std::unique_ptr<PrototypeExpressionAbstractSyntaxTree> prototype,
											 std::unique_ptr<ExpressionAbstractSyntaxTree> body) :
											prototype(std::move(prototype)), body(std::move(body)) { }

	private:
		std::unique_ptr<PrototypeExpressionAbstractSyntaxTree> prototype;
		std::unique_ptr<ExpressionAbstractSyntaxTree> body;
};

// Filled in if the token we read is an identifier.
static string identifier;
// Filled in if the token is a number. All numbers are
// doubles in this language.
static double numVal;

static int currToken;

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
		else
			return tok_identifier;
	}

	// Digit regex is [0-9.]+
	// TODO: Fix this! Will read "1.00.012..1412414" as a number.
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

		// If we are not at the end of the file, start the process over again.
		// Otherwise, it will be EOF, inwhich the next case will handle that.
		if(lastChar != EOF)
			return getToken();
	}

	if(lastChar == EOF)
		return tok_eof;

	// Returns an unknown char.
	int currChar = lastChar;
	lastChar = getchar();
	return currChar;

	return 0;
}

int getNextToken() {
	return currToken = getToken();
}

void logError(const char* err) {
	if(err)
		fprintf(stderr, "LogError: %s\n", err);
}

unique_ptr<ExpressionAbstractSyntaxTree> parseNumberExpression() {
	//auto result = make_unique<NumberExpressionAbstractSyntaxTree>(
	return nullptr;
}

/* EXPRESSION AST IMPLEMENTATIONS */
ExpressionAbstractSyntaxTree::~ExpressionAbstractSyntaxTree() {
}

int main() {
	cout << "Welcome to the Kaleidescope Language!" << endl;

	while(1) {
		cout << "> ";
		cout << getNextToken() << endl;
	}

	return 0;
}
