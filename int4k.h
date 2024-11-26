#pragma once

// int4k - Integer data type supporting up to 4096 (4K) digits

#include <iostream>

class int4k {
	friend std::istream& operator>>(std::istream& lhs, int4k& rhs);
protected:
	char digits[4096];
	int compare(const int4k& rhs) const;
	void multiply_by(char digit, int shift=0);
public:
	int4k();
	int4k(int n);
	int4k(const char* s);

	int4k& operator+=(const int4k& rhs);
	int4k& operator-=(const int4k& rhs);

	int4k operator+(const int4k& rhs) const;
	int4k operator-(const int4k& rhs) const;

	int4k operator*(const int4k& rhs) const;
	int4k& operator*=(const int4k& rhs) { *this = *this * rhs; return *this; }

	int4k& operator++() { *this += 1; return *this; }
	int4k operator++(int) { int4k tmp(*this); *this += 1; return tmp; }

	int4k& operator--() { *this -= 1; return *this; }
	int4k operator--(int) { int4k tmp(*this); *this -= 1; return tmp; }

	bool operator==(const int4k& rhs) const { return !compare(rhs); }
	bool operator!=(const int4k& rhs) const { return compare(rhs); }
	bool operator<(const int4k& rhs) const { return compare(rhs) < 0; }
	bool operator<=(const int4k& rhs) const { return compare(rhs) <= 0; }
	bool operator>(const int4k& rhs) const { return compare(rhs) > 0; }
	bool operator>=(const int4k& rhs) const { return compare(rhs) >= 0; }

	bool operator==(int rhs) const { return !compare(int4k(rhs)); }
	bool operator!=(int rhs) const { return compare(int4k(rhs)); }
	bool operator<(int rhs) const { return compare(int4k(rhs)) < 0; }
	bool operator<=(int rhs) const { return compare(int4k(rhs)) <= 0; }
	bool operator>(int rhs) const { return compare(int4k(rhs)) > 0; }
	bool operator>=(int rhs) const { return compare(int4k(rhs)) >= 0; }

	operator bool() const { return *this != int4k(); }

	char* c_str() const;
};

std::istream& operator>>(std::istream& lhs, int4k& rhs);
std::ostream& operator<<(std::ostream& lhs, const int4k& rhs);
