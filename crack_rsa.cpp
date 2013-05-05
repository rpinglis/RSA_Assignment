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

	cout << "Beginning key cracking sequence. " << endl;
	int e, c;
	cout << "Enter in public key value (e) and the corresponding (c) value:" << endl;
	cin >> e >> c;
	int d;
	d = crack_key(e, c);
	int input, output;
	char input_s[20];
	input = 0;
	bool prompts_on = true;
	cin.getline(input_s, 20); // used to clear endl from previous value read in
	while (input != 'q') {
		if (prompts_on) {
			cout << "Enter a single value (int) to decrypt, or the character 'q' to exit (or 't' to toggle these prompts): ";
			cout << endl;
		}
		
		//cin >> input;
		cin.getline(input_s, 20);
		
		if (input_s[0] == 'q') {
			cout << "Found 'q', exiting..." << endl;
			break;
		}
		else if (input_s[0] == 't') {
			cout << "Found 't', toggling prompts..." << endl;
			prompts_on = !prompts_on;
			continue;
		}
		
		input = atoi(input_s);
		
		if (prompts_on) {
			cout << "\tYou entered: " << input << endl;
		}
		
		//if ( ((char)input) == 'q') {
		//	cout << "Found 'q'" << endl;
		//	break;
		//}
		//cout << input << " " << d << " " << c << endl;
		
		output = endecrypt(input, d, c);
		
		if (prompts_on) {
			cout << "\tThis char decrypted to " << output << endl;
			cout << "\tThe letter is \t\t";
		}
		cout << "\t";
		
		// print original char
		cout << (char) output << endl;
		//cout << output << '\t' << (char) output << endl;
	}

}
