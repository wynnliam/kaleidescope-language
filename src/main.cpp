#include "./ast.h"
#include <iostream>

using namespace std;

int main() {
	cout << "Welcome to the Kaleidescope Language!" << endl;

	while(1) {
		cout << "> ";
		cout << getNextToken() << endl;
	}

	return 0;
}
