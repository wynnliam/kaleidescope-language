#include "./ast.h"
#include "./lexer.h"

static int currToken;

int getNextToken() {
	return currToken = getToken();
}

/* EXPRESSION AST IMPLEMENTATIONS */
ExpressionAbstractSyntaxTree::~ExpressionAbstractSyntaxTree() {
}


