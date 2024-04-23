#include <cstring>
#include "int4k.h"

int4k::int4k() {
	// Initialize all digits to zero
	memset(digits, 0, sizeof(digits));
}

int4k::int4k(int n) : int4k::int4k() {
	// Assumes n is positive, fill digits from right to left
	for (int i = sizeof(digits); i && n;) {
		digits[--i] = n % 10;
		n /= 10;
	}
}

int4k::int4k(const char* s) : int4k::int4k() {
	// Assume string contains only digits
	int len = strlen(s);
	memcpy(&digits[sizeof(digits) - len], s, len);
	for (int i = sizeof(digits) - len; i < sizeof(digits); i++)
		digits[i] &= 0x0f;
}

int int4k::compare(const int4k& rhs) const {
	// Compare two int4k's by comparing digits
	//	This is a protected member function used by relational operator
	//	overloads.
	// Returns:
	//	negative, if *this < rhs
	//	zero, if *this == rhs
	//	positive, if *this > rhs
	return memcmp(digits, rhs.digits, sizeof(digits));
}

char* int4k::c_str() const {

	static char ascii[sizeof(digits) + 1];

	// Find most significant digit
	int msd = 0;
	for (; !digits[msd] && msd < sizeof(digits) - 1; msd++);

	// Convert digits to ASCII
	for (int i = 0, j = msd; j < sizeof(digits); i++, j++)
		ascii[i] = digits[j] | 0x30;

	// Set null terminator
	ascii[sizeof(digits) - msd] = 0;

	return ascii;
}

void int4k::multiply_by(char digit, int shift) {
	// Multiplies by a single digit, with an optional shift of digits to the
	//	left prior to multiplication (negative shift ignored)
	if (digit) {
		if (shift > 0) {
			// Shift digits to left, fill with 0's
			memmove(digits, &digits[shift], sizeof(digits) - shift);
			memset(&digits[sizeof(digits) - shift], 0, shift);
		}
		if (digit != 1) {
			// Multiplier is >1, multiply digit-by-digit, right-to-left
			char carry = 0;
			for (int i = sizeof(digits); i;) {
				digits[i] = digits[--i] * digit + carry;
				carry = digits[i] / 10;
				digits[i] %= 10;
			}
		}
	}
	else
		// Multiplier is zero, so just set to zero
		memset(digits, 0, sizeof(digits));
}

int4k& int4k::operator+=(const int4k& rhs) {
	// Add rhs to *this, adding digit-by-digit, right-to-left
	char carry = 0;
	for (int i = sizeof(digits); i;) {
		digits[i] += (rhs.digits[--i] + carry);
		carry = digits[i] / 10;
		digits[i] %= 10;
	}
	return *this;
}

int4k& int4k::operator-=(const int4k& rhs) {
	// Subtract rhs from *this, subtracting digit-by-digit, right-to-left
	if (*this < rhs)
		// Set result to zero, since no negative number support
		memset(digits, 0, sizeof(digits));
	else {
		char borrow = 0;
		for (int i = sizeof(digits); i;) {
			digits[i] -= (rhs.digits[--i] + borrow);
			if (digits[i] >= 0)
				borrow = 0;
			else {
				digits[i] += 10;
				borrow = 1;
			}
		}
	}
	return *this;
}

int4k int4k::operator+(const int4k& rhs) {
	// Return *this + rhs (make copy of *this, then use +=)
	int4k sum(*this);
	sum += rhs;
	return sum;
}

int4k int4k::operator-(const int4k& rhs) {
	// Return *this - rhs (make copy of *this, then use -=)
	int4k diff(*this);
	diff -= rhs;
	return diff;
}

int4k int4k::operator*(const int4k& rhs) {
	// Return *this * rhs by performing a series of digit-shifts and partial
	//	products
	int4k product; // Running project
	for (int i = 0; i < sizeof(digits); i++) {
		if (rhs.digits[sizeof(digits) - i - 1]) {
			int4k partial_product(*this);
			partial_product.multiply_by(rhs.digits[sizeof(digits) - i - 1], i);
			product += partial_product;
		}
	}
	return product;
}

std::istream& operator>>(std::istream& lhs, int4k& rhs) {
	char c;

	// Get first digit
	lhs >> c;

	if (!isdigit(c)) {
		// If not a digit, put it back and set fail bit
		lhs.putback(c);
		lhs.setstate(std::ios_base::failbit);
	}
	else {
		char tmp[sizeof(rhs.digits) + 1];

		// Place left-most digit in tmp buffer
		tmp[0] = c;

		// Place succeeding digits in the buffer
		int count = 1;
		while (count < sizeof(rhs.digits) && isdigit(lhs.peek())) {
			lhs >> c;
			tmp[count++] = c;
		}
		tmp[count] = 0;

		// Convert using char* constructor
		rhs = tmp;
	}
	return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const int4k& rhs) {
	// Convert rhs to C-string and output
	lhs << rhs.c_str();
	return lhs;
}
