// Computes N! using int4k class
#include "int4k.h"
#include <iostream>
using namespace std;

int main (int argc, char* argv[]) {

	// Get value of N from command line, or prompt for N if not provided
	int4k n;
	if (argc > 1) {
		n = argv[1];
		cout << "Computing " << n << "!...\n";
	}
	else {
		cout << "Computes n!\n";
		cout << "Enter n: ";
		cin >> n;
	}

	// Compute and output n!
	int4k factorial("1");
	for (int4k i = 1; i <= n; i++)
		factorial *= i;
	cout << factorial << endl;
}
