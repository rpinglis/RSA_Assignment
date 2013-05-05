#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <cstring>

#include "rsa_tools.h"
#include "csapp.h"

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
	long clientE;
	long clientC;
	rio_t* my_rio;
	bool* thread_active;
} thread_args;

void* read_from_client(void* t_args);

int main(int argc, char **argv) {
	// global encryption variables
	long A, B, C, D, E, M, clientE, clientC;

	// thread coord variables
	bool thread_active;
	
	// socket variables
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;

	// check parameters
	if (argc != 2) {
		printf("usage: %s [port number] \n", argv[0]);
		exit(-1);
	}

	port = atoi(argv[1]); /* the server listens on a port passed via the command line */
    
	// determine public private key	
	cout << "Welcome to RSAchat server! \n" << endl;
	
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
		cout << "Okay, a set of public and private keys will be selected for you automatically:" << endl;
		int prime_a, prime_b;
		srand(time(NULL));
		//cin >> prime_a >> prime_b;
		prime_a = (rand()%168);
		prime_b = (rand()%168);
		if (prime_a == prime_b) {
			prime_b = (prime_b + 1) % 168;
		}
		//printf("prime_a: %d  prime_b: %d\n",prime_a, prime_b);
		A = primes[prime_a];
		B = primes[prime_b];
	
		cout << "Your selected primes are: A=" << A << " and B=" << B << endl;
		// determine public/private keys
		C = A*B;
		M = (A-1)*(B-1);
		E = coprime(M);
		D = mod_inverse(E, M);
	}
	cout << "Your public key is (" << E << ", " << C << ") and your private key is (" << D << ", " << C << ")" << endl;
	

	// print current usage settings
	printf("Beginning to listen for a client on Port: %d\n", port);
	 
	listenfd = open_listenfd(port); 

   while (1) {
		printf("\nWaiting for new client...");
		fflush(stdout);
		
		// set up network socket connection
		clientlen = sizeof(clientaddr); 
		connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t*) &clientlen);
		printf("Done!\n");

      hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
      haddrp = inet_ntoa(clientaddr.sin_addr);
      printf("server connected to %s (%s)\n", hp->h_name, haddrp);
      
		// set up "rio" to read from client
		rio_t rio; 
		// initialize (associate) "rio" struct with file descriptor 
		Rio_readinitb(&rio, connfd); 
	
		char buf[MAXLINE];  
		char send_buf[MAXLINE];
		
		// read in client public key (form client)
		printf("Server: reading in client public key...\n");
		Rio_readlineb(&rio, buf, MAXLINE);
		clientE = atoi(buf);
		Rio_readlineb(&rio, buf, MAXLINE);
		clientC = atoi(buf);
		printf("Read in client public key: (%d, %d)\n", clientE, clientC);

		// send server public key (to client)
		printf("Sending server public key, (%d, %d)...", E, C );
		sprintf(send_buf, "%d\n%d\n", E, C);
		
		//int msg_len = strlen(send_buf);
		Rio_writen(connfd, send_buf, strlen(send_buf));
		printf("Done!\n");

		// set up arguments to pass to "reading" thread
		thread_args* t_args = (thread_args*) malloc(sizeof(thread_args));
			t_args->connfd = connfd;
			t_args->myE = E;
			t_args->myD = D;
			t_args->myC = C;
			t_args->clientE = clientE;
			t_args->clientC = clientC;
			t_args->my_rio = &rio;
			t_args->thread_active = &thread_active;
		//printf("loop Rio: %d\n", rio);
		

		// start thread to read from client
		pthread_t read_client;
		pthread_create(&read_client, NULL, read_from_client, (void*) t_args);
		thread_active = true;

		char user_input[MAXLINE]; // encrypted_buf[MAXLINE];
		
		// print anything that user types
		while (thread_active) { 
			cout << "Please enter a message to send (type '~' to quit):" << endl;
			
			// wait for user to type a message (read into buffer)
			cin.getline(user_input, MAXLINE);
			if (!thread_active) {
				break;
			}

			int i, msg, encrypted_msg, length;
			length = strlen(user_input);
			
			// check to see if input is '~'
			if(user_input[0] == '~' && strlen(user_input) == 1){
				// send "quit" message to client
				msg = 4; // value 0x04 in ASCII corresponds to EOT (end of transmission) and would not be expected to come up in normal text
				encrypted_msg = endecrypt(msg, D, C);
				sprintf(send_buf, "%d\n", encrypted_msg);
				printf("Sending 'quit' message to client.\n");
				Rio_writen(connfd, send_buf, strlen(send_buf));
				
				// break out of while loop
				break;
			}
			
			// encrypt and send message one char at a time
			for (i=0; i<length; i++) {
				msg = user_input[i];
				encrypted_msg = endecrypt(msg, D, C);
				//itoa(msg_out, encrypted_buf, 10);
				sprintf(send_buf, "%d\n", encrypted_msg);
				printf("Writing to client, msg=%d,\tencrypted=%d,\tchar= %c\n", msg, encrypted_msg, user_input[i]);
				Rio_writen(connfd, send_buf, strlen(send_buf));
			}
			printf("Message sent!\n");
		
		}
		
		// quiting current chat
		//printf("Waiting for read_from_client thread to exit...\n");
		pthread_join(read_client, NULL);
		//printf("Thread closed.\n");
		
		//Close(connfd);  // will be closed in the function call to "talk_to_client"
    }
}


void* read_from_client(void* t_args)  
{
	//cout << "Entering threaded 'read' loop:" << endl;  // for debugging
	thread_args* my_args = (thread_args*) t_args;

	int connfd;
	bool* thread_active;
	long myE, myD, myC, clientE, clientC;
		connfd = my_args->connfd;
		myE = my_args->myE;
		myD = my_args->myD;
		myC = my_args->myC;
		clientE = my_args->clientE;
		clientC = my_args->clientC;
		thread_active = my_args->thread_active;
	free(my_args);
	
	rio_t* rio = my_args->my_rio;
	
	char buf[MAXLINE];
	//cout << "Read thread beginning. e=" << myE << " d=" << myD << " c=" << myC << " cliE=" << clientE << " cliC=" << clientC << endl;

	size_t n;  
	n = 0;
	int encrypted, decrypted;
	while(1) {
		// read until a valid message is read in
		while(n < 1) {
			n = Rio_readlineb(rio, buf, MAXLINE);
		}
		// convert input to an integer
		encrypted = atoi(buf);
		decrypted = endecrypt(encrypted, clientE, clientC);
		
		// check to see if 'quit' message received (value 4 -> EOT char in ascii)
		if (decrypted == 4) { 
			cout << "'Quit' message received from client." << endl;
			break;
		}

		// print results of decryption
		cout << "From client:";
		cout << "  encrypted: " << encrypted;
		cout << "\tdecrypted" << "  "<< decrypted << " \t" << (char)decrypted << endl;
		n = 0;
	}

	cout << "Closing connection..." << endl;
	Close(connfd);
	cout << "'read_from_client' thread exiting..." << endl << endl;
	cout << "Please press 'ENTER' to proceed..." << endl;
	*(thread_active) = false;
	pthread_exit(NULL);

}

