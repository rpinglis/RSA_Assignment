#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <cstring>

#include "rsa_tools.h"
#include "csapp.h"


// function declarations
void* write_to_server(void* t_args);


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

typedef struct {
	int connfd;
	long myE;
	long myD;
	long myC;
	long serverE;
	long serverC;
	bool* thread_active;
	bool* quit_thread;
} thread_args;


/* usage: ./echoclient host port */
int main(int argc, char **argv)
{ 
   int connfd, port; 
   char *host, *file_path, buf[MAXLINE]; 
	
	// variables used for RSA encryption
	long A, B, C, D, E, M, serverE, serverC;

	// variables for thread control
	bool thread_active, quit_thread;

	// check user input
	if (argc != 3) {
		printf("Error! Incorrect number of input arguments.\n");
		printf("\tusage: %s [host] [port]\n\n", argv[0]);
		exit(-1);
	}

	host = argv[1];
	port = atoi(argv[2]);

	// print parameters used for this run
	printf("\nUser Input:\n\tHost:\t%s\n\tPort:\t%d\n\n", host, port);

	// determine user's public/private keys
	cout << "Welcome to RSAchat client! \n" << endl;
	
	// give option for different key values
	cout << "Would you like your key values to be generated automatically? (y/n)" << endl;
	cout << "(Careful! If you enter your own, these values will not be checked for validity!)" << endl;
	char input_c;
	cin >> input_c;
	while (input_c != 'y' && input_c != 'Y' && input_c != 'n' && input_c != 'N') {
		cout << "Sorry, input not recognized. Would you like your RSA key values to be generated automatically?" << endl;
		cin >> input_c;
	}
	
	// read in public/private key pairs from user ...
	if (input_c == 'n' || input_c == 'N') {
		cout << "Please enter your public key pair (E,C). (Note: type just the numbers, type the value E first)." << endl;
		cin >> E >> C;
		cout << "Please enter your private key (D)." << endl;
		cin >> D;
	}
	
	// ... or, automatically generate public/private key pairs
	else {
		//cout << "Please select a pair of prime numbers for your key. Select the nth and mth primes (up to 168th):" << endl;
		cout << "Prime numbers are being randomly generated for you..." << endl;
		srand(time(NULL));
		int prime_a, prime_b;
		//cin >> prime_a >> prime_b;
		prime_a = rand()%168;
		prime_b = rand()%168;
		if (prime_a == prime_b) {
			prime_b = (prime_b + 1) % 168;
		}
		A = primes[prime_a];
		B = primes[prime_b];
		
		cout << "Done! Your selected primes are: A=" << A << " and B=" << B << endl;
		// determine public/private keys
		C = A*B;
		M = (A-1)*(B-1);
		E = coprime(M);
		D = mod_inverse(E, M);
	}
	
	// confirm public/private keys for user
	cout << "Your public key is (" << E << ", " << C << ") and your private key is (" << D << ", " << C << ")" << endl;

	// establish connection with specified host (on given port)
	printf("Connecting to host...");
	fflush(stdout);
   connfd = Open_clientfd(host, port);
	printf("Done!\n");
	
	// declare rio object (to read from server)
	rio_t rio;
	// associate "rio" struct with file descriptor
	rio_readinitb(&rio, connfd); 
	
	printf("Sending request...\n");
	
	// send public key (E and C) to server
	printf("Sending public key (%d, %d) to server.\n", E, C);
	char msg[MAXLINE]; 
	bzero(msg, MAXLINE);
	sprintf(msg, "%d\n%d\n", E, C);
	Rio_writen(connfd, msg, strlen(msg));
	
	// read in public key of server	
	printf("Reading in server public key:\n");
	Rio_readlineb(&rio, buf, MAXLINE);
	serverE = atoi(buf);
	Rio_readlineb(&rio, buf, MAXLINE);
	serverC = atoi(buf);	
	printf("The server's public key is (%d, %d)\n",serverE, serverC);

	// declare a thread to write to the server
	pthread_t write_server;
	thread_active = true;
	quit_thread = false;
	// set up thread_args
	thread_args* t_args = (thread_args*)malloc(sizeof(thread_args));
		t_args->connfd = connfd;
		t_args->myE = E;
		t_args->myD = D;
		t_args->myC = C;
		t_args->serverE = serverE;
		t_args->serverC = serverC;
		t_args->thread_active = &thread_active;
		t_args->quit_thread = &quit_thread;
	
	// initialize thread
	pthread_create(&write_server, NULL, write_to_server, (void*) t_args);
	//printf("Main thread: E=%d, D=%d, C=%d, serverE=%d, serverC=%d\n", E, D, C, serverE, serverC);

	long encrypted_msg, decrypted_msg;
	// enter while loop, read from server
	cout << "Entering 'read from server' loop:" << endl;
	int n = 0;
	while (thread_active) {
		// read in a line (i.e. an encrypted char)
		while (n < 1 && thread_active) {
			n = Rio_readlineb(&rio, buf, MAXLINE);
		}
		// convert to long form
		encrypted_msg = atoi(buf);
		// decrypt char msg
		decrypted_msg = endecrypt(encrypted_msg, serverE, serverC);

		// check to see if message is 'quit' message
		if (decrypted_msg == 4) { // if msg = 0x04, i.e. EOT ascii char
			printf("\nReceived 'quit' message from server. Please press 'ENTER' to disconnect...\n");
			break;
		}

		// print msg to terminal for user to read
		printf("From server: encrypted=%d \tdecrypted=%d  \tchar= %c\n", encrypted_msg, decrypted_msg, (char)decrypted_msg);
		n = 0;
	}

	// set thread flag to quit
	quit_thread = true;
	// wait for 'write_server' thread to exit
	pthread_join(write_server, NULL);
	
	// Done! 
	printf("Chat client exiting...\n");
   exit(0); 
} 


// Thread method, read user input, write to server
void* write_to_server(void* t_args) 
{
	// cout << "Entered thread!" << endl; // for debugging
	thread_args* my_args = (thread_args*) t_args;
	
	// initialize variables
	int connfd;
	bool *thread_active, *quit_thread;
	long myE, myD, myC, serverE, serverC;
		connfd = my_args->connfd;
		myE = my_args->myE;
		myD = my_args->myD;
		myC = my_args->myC;
		serverE = my_args->serverE;
		serverC = my_args->serverC;
		thread_active = my_args->thread_active;
		quit_thread = my_args->quit_thread;
	free(my_args);
	
	//printf("loop thread: E=%d, D=%d, C=%d, serverE=%d, serverC=%d\n", myE, myD, myC, serverE, serverC);
	
	// declare rio object (to read from server)
	rio_t rio;
	// associate "rio" struct with file descriptor
	rio_readinitb(&rio, connfd); 
	
	char send_buf[MAXLINE];
	char msg[MAXLINE];
	long encrypted_num;
	
	// in loop thread, write user input to server
	while(!(*quit_thread)) {
		cout << "Please enter a message to send (type '~' to quit):" << endl;
		
		// read user input
		cin.getline(msg, MAXLINE);

		// encrypt and send each character
		int i, length;
		length = strlen(msg);

		// check to see if user input 'quit' message ('~')
		if (msg[0] == '~' && length == 1) {
			printf("User entered 'quit' message.\n");
			// write quit message to server
				msg[0] = 4; // send EOT ascii char (end of transmission)
				encrypted_num = endecrypt((long)msg[0],myD,myC);
				sprintf(send_buf, "%d\n\n", encrypted_num);
				printf("Writing 'quit' message to server...\n");
				Rio_writen(connfd, send_buf, strlen(send_buf));
			
			// break out of loop
			break;
		}
		
		for (i=0; i<length; i++) {
			//printf("inside loop, i=%d\n", i);  // testing
			if (msg[i] != '\n') {
				// encrypt message using client's (my) private key
				encrypted_num = endecrypt((long)msg[i],myD,myC);
				// load encrypted char into send buffer
				sprintf(send_buf, "%d\n", encrypted_num);
				printf("Writing to server, char= %c , msg=%d, encrypted=%d\n", msg[i], msg[i], encrypted_num);
				// write to socket (send to server)
				Rio_writen(connfd, send_buf, strlen(send_buf));
			}
		}
		cout << "Message sent!" << endl;
	}
	
	// if thread is quiting because of user input (from main thread)
	if (*quit_thread) {
		msg[0] = 4; // send EOT ascii char (end of transmission)
		encrypted_num = endecrypt((long)msg[0],myD,myC);
		sprintf(send_buf, "%d\n\n", encrypted_num);
		printf("Writing 'quit' message to server...\n");
		Rio_writen(connfd, send_buf, strlen(send_buf));
	}
	
	// close socket
	Close(connfd);
	*thread_active = false;
	printf("Chat client exiting...\n");
	exit(0);
	pthread_exit(NULL);
}


