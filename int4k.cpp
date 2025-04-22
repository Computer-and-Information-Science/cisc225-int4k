#include <cstring>
#include "int4k.h"

int4k::int4k() {
	// Initialize all digits to zero
	memset(digits, 0, sizeof(digits));
}

int4k::int4k(int n) : int4k::int4k() {
	// Check for negative number
	bool negative = n < 0;
	if (negative)
		n = -n;
	// Fill digits from right to left (positive integer)
	for (char *p = digits + sizeof(digits) - 1; n; --p) {
		*p = n % 10;
		n /= 10;
	}
	// Change sign if negative
	if (negative) neg();
}

int4k::int4k(const char* s) : int4k::int4k() {
	// Assume string contains only digits and optional leading sign
	int len = strlen(s);
	// Check for leading sign
	bool negative = false;
	if (*s == '+' || *s == '-') {
		negative = *s == '-';
		memcpy(&digits[sizeof(digits) - len + 1], s + 1, len - 1);
	}
	else
		memcpy(&digits[sizeof(digits) - len], s, len);
	// Convert ASCII to BCD
	for (char *p = digits + sizeof(digits) - len; p != digits + sizeof(digits); ++p)
		*p &= 0x0f;
	// Change sign if negative
	if (negative) neg();
}

int int4k::compare(const int4k& rhs) const {
	// Compare two int4k's by comparing digits
	//	This is a protected member function used by relational operators
	//	overloads.
	// Returns:
	//	negative, if *this < rhs
	//	zero, if *this == rhs
	//	positive, if *this > rhs
	if (is_negative() != rhs.is_negative())
		return is_negative() ? -1 : 1;
	return is_negative() ? memcmp(rhs.digits, digits, sizeof(digits)) : memcmp(digits, rhs.digits, sizeof(digits));
}

char* int4k::c_str() const {

	// For conversion to ASCII, accounting for newline and minus sign
	static char ascii[sizeof(digits) + 2];

	// Find absolute value as a temporary int4k object
	int4k tmp(*this); tmp.abs();

	// Find most significant digit, set pointer to that digit
	char *p = tmp.digits;
	for (; !*p && p < tmp.digits + sizeof(digits) - 1; ++p);

	// Pointer to destination C string
	char *q = ascii;	

	// If negative, set first C string character to minus sign
	if (is_negative())
		*q++ = '-';

	// Convert digits to ASCII
	for (; p < tmp.digits + sizeof(digits); ++p, ++q)
		*q = *p | 0x30;

	// Set null terminator
	*q = 0;

	return ascii;
}

std::pair<int4k, int4k> int4k::divide_by(const int4k& divby) const {

	// Initialize dividend and divisor to absolute values of *this and divby
	int4k dividend(*this); dividend.abs();
	int4k divisor(divby); divisor.abs();

	// Find most significant digits of dividend and divisor
	int msd_dividend = 0; for (; !dividend.digits[msd_dividend] && msd_dividend < sizeof(digits) - 1; msd_dividend++);
	int msd_divisor = 0; for (; !divisor.digits[msd_divisor] && msd_divisor < sizeof(digits) - 1; msd_divisor++);

	// Initialize quotient and remainder to zero and the dividend
	int4k quotient;
	int4k remainder(dividend);

	// Shift divisor left to align MSD with dividend
	int shift = msd_dividend < msd_divisor ? msd_divisor - msd_dividend : 0;
	divisor.shl(shift);

	// Initialize partial quotient to power of 10 based on shift of divisor
	int4k partial_quotient = int4k(1).shl(shift);

	// Carry out the division
	do {
		while (divisor <= remainder) {
			quotient += partial_quotient;
			remainder -= divisor;
		}
		divisor.shr(1);
		partial_quotient.shr(1);
	} while (--shift >= 0);

	// Adjust signs of quotient and remainder
	if (is_negative() != divby.is_negative())
		quotient.neg();
	if (is_negative())
		remainder.neg();

	// Return quotient and remainder as a pair
	return std::pair<int4k, int4k>(quotient, remainder);
}

void int4k::multiply_by(char digit, int shift) {
	// Multiplies by a single digit, with an optional shift of digits to the
	//	left prior to multiplication (negative shift ignored)
	// Precondition: *this and digit are non-negative
	if (digit) {
		shl(shift);
		if (!digit)
			// Multiplier is zero, just set to zero
			memset(digits, 0, sizeof(digits));
		else if (digit != 1) {
			// Multiplier is >1, multiply digit-by-digit, right-to-left
			char carry = 0;
			for (char *p = digits + sizeof(digits); p > digits;) {
				(*--p *= digit) += carry;
				carry = *p / 10;
				*p %= 10;
			}
		}
	}
	else
		// Multiplier is zero, so just set to zero
		memset(digits, 0, sizeof(digits));
}

int4k& int4k::shl(int shift) {
	// Precondition: *this is non-negative
	if (shift) {
		memmove(digits, &digits[shift], sizeof(digits) - shift);
		memset(&digits[sizeof(digits) - shift], 0, shift);
	}
	return *this;
}

int4k& int4k::neg() {
	for (char *p = digits; p < digits + sizeof(digits); ++p)
		*p = 9 - *p;
	return ++(*this);
}

int4k& int4k::shr(int shift) {
	// Precondition: *this is non-negative
	if (shift) {
		bool negative = is_negative();
		memmove(&digits[shift], digits, sizeof(digits) - shift);
		memset(digits, 0, shift);
	}
	return *this;
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

int4k int4k::operator*(const int4k& rhs) const {
	// Return *this * rhs by performing a series of digit-shifts and partial
	//	products
	int4k multiplicand(*this); multiplicand.abs();
	int4k multiplier(rhs); multiplier.abs();
	int4k product; // Running project
	for (int i = 0; i < sizeof(digits); i++) {
		if (multiplier.digits[sizeof(digits) - i - 1]) {
			int4k partial_product(multiplicand);
			partial_product.multiply_by(multiplier.digits[sizeof(digits) - i - 1], i);
			product += partial_product;
		}
	}
	return is_negative() == rhs.is_negative() ? product : product.neg();
}

std::istream& operator>>(std::istream& lhs, int4k& rhs) {
	char c;

	// Get first digit or sign
	lhs >> c;

	bool negative = false;
	if (c == '-' || c == '+') {
		negative = c == '-';
		lhs >> c;
	}

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

		// Change sign if negative
		if (negative)
			rhs = -rhs;
	}
	return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const int4k& rhs) {
	// Convert rhs to C-string and output
	lhs << rhs.c_str();
	return lhs;
}
