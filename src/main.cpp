#include "./lexer.h"
#include "./ast.h"
#include <iostream>

using namespace std;

int main() {
	cout << "Welcome to the Kaleidescope Language!" << endl;

	unique_ptr<NumberExpressionAbstractSyntaxTree> num1 = make_unique<NumberExpressionAbstractSyntaxTree>(10);
	unique_ptr<NumberExpressionAbstractSyntaxTree> num2 = make_unique<NumberExpressionAbstractSyntaxTree>(20);
	BinaryExpressionAbstractSyntaxTree bin('+', std::move(num1), std::move(num2));

	while(1) {
		cout << "> ";
		cout << getToken() << endl;
	}

	return 0;
}
