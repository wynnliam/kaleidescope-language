#include "./ast.h"
#include "./lexer.h"
#include <cstdio>

static int currToken;

int getNextToken() {
	return currToken = getToken();
}

void logError(const char* err) {
	if(err)
		fprintf(stderr, "LogError: %s\n", err);
}

/* EXPRESSION AST IMPLEMENTATIONS */
ExpressionAbstractSyntaxTree::~ExpressionAbstractSyntaxTree() {
}


