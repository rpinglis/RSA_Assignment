#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <iostream>

#include "rsa_tools.h"

#define min(a,b) (a<=b)*(a)+(b<a)*(b)
#define max(a,b) (a>=b)*(a)+(b>a)*(b)

using namespace std;

/* using rand, picks a random integer that is coprime to the given input parameter (but less than the given input) */
long coprime(long x) 
{
	srand(time(NULL));
	
	long start = rand() % x;
	if (start == 0 | start == 1 ) {
		start = 2;
	}
	long result = start;
	while ( GCD(result, x) != 1) {
		result = (result+1) % x;
		if (result == 0 || result == 1) {
			result = 2;
		}
		if (result == start) {
			cout << "Sorry, coprime does not exist in range [1," << x <<"]" << endl;
			return -1;
		}
	}
	return result;
}

/* given a integer repreresenting an ASCII character value, encrypt it via the RSA crypto algorithm */
int endecrypt (int msg_or_cipher, long key, long c)
{
	return modulo(msg_or_cipher, key, c);
}

/* computes the GCD of two numbers a and b */
/* REF:  http://en.wikipedia.org/wiki/Greatest_common_divisor */
long GCD (long a, long b)
{
	//cout << "GCD(" << a << ", " << b << ") " << endl;
	long min = min(a,b);
	long max = max(a,b);

	//cout << "(b<a)" << (b<a) << endl;

	//cout << "min: " << min << endl;
	//cout << "max: " << max << endl;
	
	if (min == 0) {
		//cout << "Result == max: " << max << endl;
		return max;
	}

	long result = GCD(min, max % min);
	//cout << "Result: " << result << endl;

	return result;
}

/* compute the modular inverse base^-1 % m */
long mod_inverse (long base, long m)
{
	// assume that "base" and "m" are co-prime 
	// (NOTE: inverse will only exist if base and m are co-prime
	// (if not co-prime, other numbers may exist in the set [1, m-1] which fulfill the conditions)
	// (may return one of them instead)
	
	if (GCD(base, m) != 1) {
		cout << "mod_inverse:: Eror, mod inverse does not exist, numbers are not co-prime" << endl;
		return -2;
	}
	
	// brute force implementation
	long c;
	long result;
	for (c = 1; c < m; c++) {
		result = base * c;
		result = result % m;
		if (result == 1) {
			return c;
		}
	}
	if (c == m) {
		return -1;
	}
	return c;
}

/* computes Math.mod(Math.pow(a, b), c), for large values of a, b, and c */
long modulo (long a, long b, long c)
{
	// we want to compute a^b % c
	// can be performed by performing modulo operationon the number after 
	// each "round" of multiplication for the section computing the power
	
	long result, counter;
	result = 1;
	for (counter = 0; counter < b; counter++) {
		result = result * a;
		result = result % c;
	}
	return result;
}

/* compute Euler's Totient */
long totient (long n)
{
	// brute force implementation
	long i;
	long counter = 0;
	for (i = 1; i<n; i++) {
		//cout << "i = " << i << endl;
		if (GCD(i, n) == 1) {
			counter++;
		}
	}
	return counter;
}

/* given values for 'e' and 'c', will crack the key and find 'd' */
long crack_key(long e, long c) 
{
	// first, find 'a' and 'b'
	long a, b;
	for (a = 2; a < c; a++) {
		if (c % a == 0) {
			break;
		}
	}
	b = c / a;

	cout << "Found original primes.  a = " << a << "  b = " << b << endl;

	// next, find 'd' (decryption key)
	// calculate 'm'
	long m;
	m = (a-1)*(b-1);

	// now recompute 'd'
	long d;
	d = mod_inverse(e, m);

	cout << "Found decryption key 'd'.  d = " << d << endl;

	return d;
}

