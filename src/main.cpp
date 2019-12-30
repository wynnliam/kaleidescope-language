#include "./lexer.h"
#include "./ast.h"
#include <iostream>

using namespace std;

int main() {
	cout << "Welcome to the Kaleidescope Language!" << endl;

	NumberExpressionAbstractSyntaxTree num(10);
	VariableExpressionAbstractSyntaxTree var("awdaw");

	while(1) {
		cout << "> ";
		cout << getToken() << endl;
	}

	return 0;
}
