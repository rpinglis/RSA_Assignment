#ifndef __RSA_TOOLS
#define __RSA_TOOLS

using namespace std;

/* using crandom, picks a random integer that is coprime to the given input parameter */
long coprime(long x);

/* given a integer repreresenting an ASCII character value, encrypt it via the RSA crypto algorithm */
int endecrypt (int msg_or_cipher, long key, long c);

/* computes the GCD of two numbers a and b */
long GCD (long a, long b);

/* compute the modular inverse base^-1 % m */
long mod_inverse (long base, long m);

/* computes Math.mod(Math.pow(a, b), c), for large values of a, b, and c */
long modulo (long a, long b, long c);

/* compute Euler's Totient */
long totient (long n);

/* given values for 'e' and 'c', will crack the key and find 'd' */
long crack_key(long e, long c);

#endif
