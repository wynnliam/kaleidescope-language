#include "./ast.h"
#include "./lexer.h"
#include <cstdio>

using namespace std;

static int currToken;

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


