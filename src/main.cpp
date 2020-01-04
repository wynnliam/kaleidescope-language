#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <map>

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

static map<char, int> binOpPrecedence;

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

int getTokenPrecedence() {
	if(!isascii(currToken))
		return -1;

	int precedence = binOpPrecedence[currToken];
	if(precedence <= 0)
		return -1;

	return precedence;
}

void logError(const char* err) {
	if(err)
		fprintf(stderr, "LogError: %s\n", err);
}

unique_ptr<ExpressionAbstractSyntaxTree> parseNumberExpression();
unique_ptr<ExpressionAbstractSyntaxTree> parseBinaryOpRHS(int expPrecedence, unique_ptr<ExpressionAbstractSyntaxTree> lhs);
unique_ptr<ExpressionAbstractSyntaxTree> parseParanthesisExpression();
unique_ptr<ExpressionAbstractSyntaxTree> parseIdentifierExpression();
unique_ptr<ExpressionAbstractSyntaxTree> parsePrimary();
unique_ptr<ExpressionAbstractSyntaxTree> parseExpression();

// Parses a number expression, advances lexer by a token, and returns resulting
// number AST
unique_ptr<ExpressionAbstractSyntaxTree> parseNumberExpression() {
	auto result = make_unique<NumberExpressionAbstractSyntaxTree>(numVal);
	getNextToken(); // Consume the number
	return std::move(result);
}

// Parses expression of form '( EXPR )'
unique_ptr<ExpressionAbstractSyntaxTree> parseParanthesisExpression() {
	getNextToken(); // Eat the '('

	// parseExpression could call this function. Thus, it shows
	// how we can handle recursive grammars.
	auto result = parseExpression();
	if(!result)
		return nullptr;

	if(currToken != ')') {
		logError("Expected ')'");
		return nullptr;
	}

	getNextToken(); // Consume the ')'

	return result;
}

// Handles expressions that reference a variable identifier or
// an expression of the form '( expression ... )'
unique_ptr<ExpressionAbstractSyntaxTree> parseIdentifierExpression() {
	string idName = identifier;

	getNextToken(); // Eat the identifier.

	// Simple variable reference
	if(currToken != '(')
		return make_unique<VariableExpressionAbstractSyntaxTree>(idName);

	getNextToken(); // Eats the '('

	vector<unique_ptr<ExpressionAbstractSyntaxTree>> args;
	if(currToken != ')') {
		while(1) {
			if(auto arg = parseExpression())
				args.push_back(move(arg));
			else
				return nullptr;

			if(currToken == ')')
				break;

			if(currToken != ',') {
				logError("Expected ')' or ',' in argument list");
				return nullptr;
			}

			getNextToken();
		}
	}

	// Eat the ')'
	getNextToken();

	return make_unique<CallExpressionAbstractSyntaxTree>(idName, std::move(args));
}

// Primary expressions are either identifier expressions, number expressions, or parenthesis expressions.
unique_ptr<ExpressionAbstractSyntaxTree> parsePrimary() {
	switch(currToken) {
		case tok_identifier:
			return parseIdentifierExpression();
		case tok_number:
			return parseNumberExpression();
		case '(':
			return parseParanthesisExpression();
		default:
			logError("Unknown token when expecting expression");
			return nullptr;
	}
}

// Parses expressions of the form 'primary binOps' where
// binOps is zero or more binary operator expressions.
unique_ptr<ExpressionAbstractSyntaxTree> parseExpression() {
	auto lhs = parsePrimary();

	if(!lhs)
		return nullptr;

	return parseBinaryOpRHS(0, move(lhs));
}

// Parses expressions of the form (operator primaryExpression)*
unique_ptr<ExpressionAbstractSyntaxTree> parseBinaryOpRHS(int expPrecedence, unique_ptr<ExpressionAbstractSyntaxTree> lhs) {
	int tokPrecedence;
	int binOp;

	while(1) {
		tokPrecedence = getTokenPrecedence();

		// If we're dealing with a binary operation at least as tight as the current binop, then consume it.
		// Otherwise we are done.
		if(tokPrecedence < expPrecedence)
			return lhs;

		// We know it's a binary operation, so save the operation
		// and get the next token.
		binOp = currToken;
		getNextToken();

		// Parse the binary expression after the operator.
		auto rhs = parsePrimary();
		if(!rhs)
			return nullptr;

		// TODO: Finish me!
	}
}

/* EXPRESSION AST IMPLEMENTATIONS */
ExpressionAbstractSyntaxTree::~ExpressionAbstractSyntaxTree() {
}

int main() {
	cout << "Welcome to the Kaleidescope Language!" << endl;

	// Set the operator precedence
	binOpPrecedence['<'] = 1;
	binOpPrecedence['+'] = 2;
	binOpPrecedence['-'] = 2;
	binOpPrecedence['*'] = 3;

	while(1) {
		cout << "> ";
		cout << getNextToken() << endl;
	}

	return 0;
}
