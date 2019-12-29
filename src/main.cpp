#include "./lexer.h"
#include <iostream>

using namespace std;

int main() {
	cout << "Welcome to the Kaleidescope Language!" << endl;

	while(1) {
		cout << "> ";
		cout << getToken() << endl;
	}

	return 0;
}
