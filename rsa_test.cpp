#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <climits>
#include <cstring>

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
	// test coprime
/*	cout << "Testing 'coprime' " << endl;
	cout << "Enter a number to test: ";
	int num1;
	cin >> num1;
	cout << "Coprime number is : " << coprime(num1) << endl;
*/

	// test GCD 
/*	cout << "Testing 'GCD' function: " << endl;
	int num1, num2;
	cout << "Enter two numbers to find the GCD of: ";
	cin >> num1 >> num2;
	
	cout << "Read in " << num1 << " and " << num2 << endl;
	cout << "GCD of the two is: " << GCD(num1, num2);
*/


	// test mod_inverse
/*	cout << "Testing 'mod_inverse' " << endl;
	cout << "Enter a base number and an m number to test: ";
	int base, m;
	cin >> base >> m;
	cout << "You enetered: base=" << base << " m=" << m << endl;
	cout << "mod_inverse: " << mod_inverse(base,m) << endl;
*/


	// test modulo
 /*	
	cout << "Testing 'modulo' " << endl;
	cout << "Enter numbers for: base, exponent, modulos: ";
	int base, exp, mod;
	cin >> base >> exp >> mod;
	cout << "You entered: base=" << base << " exp=" << exp << " mod=" << mod << endl;
	cout << "modulo: " << modulo(base,exp,mod) << endl;
	//cout << "modulo: " << endecrypt(base,exp,mod) << endl;
// */


	// test totient
/*	cout << "Testing 'totient' " << endl;
	cout << "Enter input number: ";
	int num1;
	cin >> num1;
	cout << "You entered: " << num1 << endl;
	cout << "totient: " << totient(num1) << endl;
*/


	// test key cracking
///*	
	cout << "Beginning key cracking sequence. " << endl;
	int e, c;
	cout << "Enter in public key value (e) and the corresponding (c) value:" << endl;
	cin >> e >> c;
	int d;
	d = crack_key(e, c);
	int input, output;
	char input_s[20];
	input = 0;
	cin.getline(input_s, 20); // used to clear endl from previous value read in
	while (input != 'q') {
		cout << "Enter a value (int) to decrypt, or the character 'q' to exit: ";
		//cin >> input;
		cin.getline(input_s, 20);
		if (input_s[0] == 'q') {
			cout << "Found 'q'" << endl;
			break;
		}
		input = atoi(input_s);
		cout << "You entered: " << input << endl;
		//if ( ((char)input) == 'q') {
		//	cout << "Found 'q'" << endl;
		//	break;
		//}
		//cout << input << " " << d << " " << c << endl;
		output = endecrypt(input, d, c);
		cout << "This char decrypted to " << output << endl;
		cout << "The letter is " << (char) output << endl;
		//cout << output << '\t' << (char) output << endl;
	}
// */


	// test key encryption
/*	
	int e, c;
	char input_msg[30];
	cout << "Enter the public key (e, c) you would like to use: " << endl;
	cin >> e >> c;
	cout << "Please enter a message to encrypt: ";
	cin.getline(input_msg, 30); // clear '\n' form prev input
	cin.getline(input_msg, 30);
	int i, output, msg;
	int length;
	length = strlen(input_msg);
	cout << "You wrote:  " << input_msg << endl;
	//cout << 
	for (i=0; i<length; i++) {
		// encrypt single char
		msg = (int) input_msg[i];
		cout << i << "  " << input_msg[i] << "  " << msg << "  -->\t";
		output = (int)endecrypt((long)msg, (long)e, (long)c);
		//output = modulo(msg, e, c);
		cout << output << endl;
	}
// */

	// Done!
	cout << endl << "Testing complete. Exiting." << endl;
}
