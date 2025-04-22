// Computes N! using int4k class
#include "int4k.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

int main (int argc, char* argv[]) {

	cout << "Testing int4k operators...\n";

	int4k a, b;
	int x, y;

	cout << "Addition ";
	for (int i = 0; i < 60; i++) {
		x = rand() % 1000000000;
		y = rand() % 1000000000;
		if (rand() % 2) x = -x;
		if (rand() % 2) y = -y;
		a = x;
		b = y;
		cout << '.'; cout.flush();
		if (a + b != x + y) {
			cout << " failed " << x << '+' << y << '=' << a + b;
			break;
		}
	}
	cout << '\n';

	cout << "Subtraction ";
	for (int i = 0; i < 60; i++) {
		x = rand() % 1000000000;
		y = rand() % 1000000000;
		if (rand() % 2) x = -x;
		if (rand() % 2) y = -y;
		a = x;
		b = y;
		cout << '.'; cout.flush();
		if (a - b != x - y) {
			cout << " failed " << x << '-' << y << '=' << x - y << '|' << a << '-' << b << '=' << a - b;
			break;
		}
	}
	cout << '\n';

	cout << "Multiplication ";
	for (int i = 0; i < 60; i++) {
		x = rand() % 10000;
		y = rand() % 10000;
		if (rand() % 2) x = -x;
		if (rand() % 2) y = -y;
		a = x;
		b = y;
		cout << '.'; cout.flush();
		if (a * b != x * y) {
			cout << " failed " << x << '*' << y << '=' << x * y << '|' << a << '*' << b << '=' << a * b;
			break;
		}
	}
	cout << '\n';

	cout << "Division ";
	for (int i = 0; i < 60; i++) {
		x = rand() % 1000000000;
		y = rand() % 10000;
		if (rand() % 2) x = -x;
		if (rand() % 2) y = -y;
		a = x;
		b = y;
		cout << '.'; cout.flush();
		if (a / b != x / y) {
			cout << " failed " << x << '/' << y << '=' << x / y << '|' << a << '/' << b << '=' << a / b;
			break;
		}
	}
	cout << '\n';

	cout << "Modulus ";
	for (int i = 0; i < 60; i++) {
		x = rand() % 1000000000;
		y = rand() % 10000;
		if (rand() % 2) x = -x;
		if (rand() % 2) y = -y;
		a = x;
		b = y;
		cout << '.'; cout.flush();
		if (a % b != x % y) {
			cout << " failed " << x << '%' << y << '=' << x % y << '|' << a << '%' << b << '=' << a % b;
			break;
		}
	}
	cout << '\n';

	cout << "IsEqual ";
	for (int i = 0; i < 60; i++) {
		x = rand() % 1000000000;
		if (rand() % 2) x = -x;
		a = x;
		b = x;
		cout << '.'; cout.flush();
		if (a == b && a == x);
		else {
			cout << " failed " << a << " == " << b << "\n";
			break;
		}
	}
	cout << '\n';

	cout << "IsNotEqual ";
	for (int i = 0; i < 60; i++) {
		x = rand() % 1000000000;
		y = rand() % 10000;
		if (rand() % 2) x = -x;
		if (rand() % 2) y = -y;
		a = x;
		b = y;
		cout << '.'; cout.flush();
		if (x != y && a != b);
		else {
			cout << " failed " << x << "!=" << y << '|' << a << "!=" << b;
			break;
		}
	}
	cout << '\n';

	cout << "100! .";
	char f100[] = "93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000";
	int4k f = 1;
	for (int n = 1; n <= 100; n++)
		f *= n;
	if (strcmp(f.c_str(), f100))
		cout << " failed 100! != " << f;
	cout << '\n';

	cout << "char* constructor .";
	f = int4k(f100);
	if (strcmp(f.c_str(), f100))
		cout << " failed " << f;
	cout << '.';
	f100[0] = '-';
	f = int4k(f100);
	if (strcmp(f.c_str(), f100))
		cout << " failed " << f;
	cout << '\n';
}
