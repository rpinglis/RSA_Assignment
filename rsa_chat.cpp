#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <climits>

#include "rsa_tools.h"

using namespace std; 

// smallest 168 primes (all primes < 1000)
// taken from <http://en.wikipedia.org/wiki/Prime_number>
int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 
					71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 
					151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
					233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 
					317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 
					419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 
					503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 
					607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 
					701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 
					811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 
					911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 };

int main (int argc, char** argv) 
{
	int mth_prime, nth_prime;
	// compute two different prime numbers 'a' and 'b'
	cout << "Enter the nth and mth prime to compute: " << endl;
	cin >> mth_prime >> nth_prime;
	
	int a, b;
	a = primes[mth_prime];
	b = primes[nth_prime];
	cout << endl << "You picked: " << a << " and " << b << endl;
	
	// computer c = a*b and m = (a-1)*(b-1)
	int c, m;
	c = a * b;
	m = (a-1) * (b-1);
	
	// compute e, the encryption key (any number coprime to m)
	int e;
	e = coprime(m);
	
	// compute d, the modular inverse of e mod m
	int d;
	d = mod_inverse(e, m);
	
	// encrypt any number x (encryption: y = x^e mod c
	int x;
	cout << "Enter a number to be encrypted: " << endl;
	//cin.ignore(INT_MAX);
	cin >> x;
	
	int y;
	y = endecrypt(x, e, c);
	cout << "You entered " << x << ". Encrypted that number becomes " << y << endl;
	
	// decrypt the number y (encryption: x = y^d mod c
	cout << "Decryption:" << endl;
	cout << "Given encrypted input " << y << ". Now decrypting..." << endl;
	int result;
	result = endecrypt(y, d, c);
	cout << "Done! The original message was " << result << "." << endl;
	
	cout << "Program finished. Now exiting." << endl;
	
	return 0;
}
