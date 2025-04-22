#!/usr/bin/env python3

n = int(input('Enter n: '))
factorial = 1
for i in range(2, n + 1):
	factorial *= i
print(factorial)
