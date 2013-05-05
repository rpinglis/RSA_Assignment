Paul Inglis
CIT 595 ->   A6 (RSA Chat Assignment)
/******************************************************************************************************************/
/******************************************************************************************************************/
APPROACH

  RSA chat:

    For server or client, the first step is to determine the individual's public/private key pairs. This can be done eithe rautomatically, or the user can manually enter a pre-selected set of values (though these will not be checked by the program). Keys which are generated automatically are created using two different primes randomly picked from the first 168 primes (i.e. all primes under 1000). This method follows the process for generating public/private key pairs outlined in the assignment description.

    The server should be started first and waits until it gets a request from a client. Once that happens, the client sends its public key pair to the server (unencrypted). The server then sends its public key to the client (also unencrypted). 

    On the client side, the program spawns a second thread to handle user input (to be written to the server) while the main thread reads from the socket (i.e. from the server) and prints to the screen. On the server side, the opposite occurs. A new thread is created to read from the socket (i.e. from the client), while the main thread handles user input (and sends it, encrypted, to the client).

    The chat program then begins in earnest.  Either user (client/server) can send a message asynchronously to the other (server/client). Upon hitting the carriage return, the message which the user typed will be encrypted (using the other user's public key) and sent across the socket connection one character at a time. Each encrypted character line delineated and is read in on the other side of the socket connection by the server/client. Each character is then decrypted using the receiving user's private key, and printed to the screen. 
    (*** It should be noted that additionaly information is also printed for each character being encrypted/decrypted, sent/received for the purpose of making the RSA encryption process more visible.)

    One item of note, the chat server/client doesn't really have a 'clean' exit implemented. There is an exit sequence, but due to the multiple threads being used and the way that a request to close the chat is communicated across the socket, it takes a bit of extra steps to fully disengage the chat client. Alternatively, the user can manually disengage by pressing the old standby: ctrl + C.


  RSA key cracker:
    
    The key cracking algorithm takes advantage of the fact that we know the public key (E, C), and that C is the product of two prime numbers. Thus, a brute force approach can be implemented by attempting to divide the number C by every number small than it (or, or better efficiency, by every prime smaller than it). When a number, A, is found which can be divided into C evenly, we know that we have found one of the two originaly primes used to generate the key pair. The other prime, B, is then B = C/ A.

    From there, it's pretty simple for use to recreate the private key, following the method:
      M = (A-1)*(B-1)
      D = mod_inverse(E, M)
    where "mod_inverse()" is a method which takes the modular multiplicative inverse of E, modulated by M. Our result, D, is half of the private key which we can use to decrypt the encrypted message. From there, having discovered the private key pair, (D, C), we easily use the regular RSA method to crack our message. Nice!
  
  *** NOTE: The key cracked as an example was the public key and encrypted message posted on Piazza by anonymous for people working individually. Reproduced here:

  "Here is a message to decrypt, if anyone wants it to test their codebreaking:
  C = 9271, e = 4793
  Message:
  6355 1333 8077 2683 1668 2339 7626 8872 2683 2728 8872 1053 3230 3527 7451 1001 8872 2728 2683 4510 3527 2683 4510 8872 6946 6946 2339 6372 8872 6185"
    

/******************************************************************************************************************/
STRUCTURES

  Hmm, really nothing special this time around. Maybe, for the chat program it's worth mentioning that server and client each implement threading, and need to be able to handle socket communication.

  On the RSA key cracker, there really weren't any particualr structures used.

  

/******************************************************************************************************************/
CODE

  client.cpp
    Code for the RSA chat client. Run the server first, then try to connect with the client (make sure to specify the same port number and network). Allows option to input your own public/private key or have it automatically generated for you (note: will not verify that the numbers you input are actually a valid key pairing). 

  server.cpp
    Code for the RSA chat server. As above, remember to run the server first, then try to connect with the client (on the server you only need to specify the port number). Again, allows option to input your own public/private key or have it automatically generated for you (note: will not verify that the numbers you input are actually a valid key pairing). 

  rsa_tools.cpp
    Contains all of the methods used in the RSA programs (i.e. coprime(), endecrypt(), GCD(), mod_inverse(), modulo(), totient(). )

  crack_rsa.cpp
    Contains the code used in the RSA key cracker program. It should be noted that this function is not part of the actual RSA chat program.
 
  csapp.cpp
    This is the helpful library which Prof. Mongan recommended we include. The chat program makes use of it to simplify socket connections.

  rsa_test.cpp
    Not part of the actual assignment, but code foa a small program which I used to test the different RSA functionalities included in rsa_tools.cpp (most of the tests are currently commented out).

/******************************************************************************************************************/
PROGRAM USAGE

  RSA chat (server) (1 of 2):
    
    $ 	make server
    $ 	server_rsa [port number]

    ex: $  server 8812
      
      
  RSA chat (client) (2 of 2):
  
    $ 	make client
    $ 	client_rsa [host network] [port number]

    ex: $  client minus.seas.upenn.edu 8812
      
      

  RSA key cracker:
      
    $	make crack_rsa
    $	crack_rsa
    
    --> follow subsequent program instructions


/******************************************************************************************************************/
RESULTS

------------------------------------------------------------------------------------------------------------------
RSA Chat (server): -----------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------


ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> make all
g++ -g -lpthread  rsa_tools.cpp csapp.cpp server.cpp -o server_rsa
g++ -g -lpthread  rsa_tools.cpp csapp.cpp client.cpp -o client_rsa
g++ -g -lpthread  rsa_tools.cpp rsa_test.cpp -o rsa_test
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> ls
client.cpp  crack_rsa      csapp.cpp  makefile    README.txt~   rsa_test      rsa_tools.cpp  server.cpp
client_rsa  crack_rsa.cpp  csapp.h    README.txt  rsa_chat.cpp  rsa_test.cpp  rsa_tools.h    server_rsa
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> server_rsa 8812
Welcome to RSAchat server! 

Would you like your key values to be generated automatically? (y/n)
(Careful! If you enter your own, these values will not be checked for validity!)
y
Okay, a set of public and private keys will be selected for you automatically:
Your selected primes are: A=853 and B=827
Your public key is (237701, 705431) and your private key is (693893, 705431)
Beginning to listen for a client on Port: 8812

Waiting for new client...Done!
server connected to TEPIDITY.SEAS.UPENN.EDU (158.130.63.15)
Server: reading in client public key...
Read in client public key: (9467, 9959)
Sending server public key, (237701, 705431)...Done!
Please enter a message to send (type '~' to quit):
Message sent!
Please enter a message to send (type '~' to quit):
From client:  encrypted: 9048   decrypted  73   I
From client:  encrypted: 6292   decrypted  32    
From client:  encrypted: 9667   decrypted  97   a
From client:  encrypted: 5893   decrypted  109  m
From client:  encrypted: 6292   decrypted  32    
From client:  encrypted: 553    decrypted  116  t
From client:  encrypted: 8551   decrypted  121  y
From client:  encrypted: 2340   decrypted  112  p
From client:  encrypted: 2992   decrypted  105  i
From client:  encrypted: 6870   decrypted  110  n
From client:  encrypted: 797    decrypted  103  g
From client:  encrypted: 6292   decrypted  32    
From client:  encrypted: 553    decrypted  116  t
From client:  encrypted: 8104   decrypted  104  h
From client:  encrypted: 2992   decrypted  105  i
From client:  encrypted: 2967   decrypted  115  s
From client:  encrypted: 6292   decrypted  32    
From client:  encrypted: 4506   decrypted  102  f
From client:  encrypted: 2992   decrypted  105  i
From client:  encrypted: 413    decrypted  114  r
From client:  encrypted: 2967   decrypted  115  s
From client:  encrypted: 553    decrypted  116  t
Okay, now I am typing this second                
Writing to client, msg=79,      encrypted=422700,       char= O
Writing to client, msg=107,     encrypted=341337,       char= k
Writing to client, msg=97,      encrypted=543527,       char= a
Writing to client, msg=121,     encrypted=628716,       char= y
Writing to client, msg=44,      encrypted=144310,       char= ,
Writing to client, msg=32,      encrypted=357146,       char=  
Writing to client, msg=110,     encrypted=641211,       char= n
Writing to client, msg=111,     encrypted=300081,       char= o
Writing to client, msg=119,     encrypted=382565,       char= w
Writing to client, msg=32,      encrypted=357146,       char=  
Writing to client, msg=73,      encrypted=167733,       char= I
Writing to client, msg=32,      encrypted=357146,       char=  
Writing to client, msg=97,      encrypted=543527,       char= a
Writing to client, msg=109,     encrypted=32882,        char= m
Writing to client, msg=32,      encrypted=357146,       char=  
Writing to client, msg=116,     encrypted=497469,       char= t
Writing to client, msg=121,     encrypted=628716,       char= y
Writing to client, msg=112,     encrypted=526460,       char= p
Writing to client, msg=105,     encrypted=681622,       char= i
Writing to client, msg=110,     encrypted=641211,       char= n
Writing to client, msg=103,     encrypted=227500,       char= g
Writing to client, msg=32,      encrypted=357146,       char=  
Writing to client, msg=116,     encrypted=497469,       char= t
Writing to client, msg=104,     encrypted=220567,       char= h
Writing to client, msg=105,     encrypted=681622,       char= i
Writing to client, msg=115,     encrypted=647985,       char= s
Writing to client, msg=32,      encrypted=357146,       char=  
Writing to client, msg=115,     encrypted=647985,       char= s
Writing to client, msg=101,     encrypted=483757,       char= e
Writing to client, msg=99,      encrypted=74667,        char= c
Writing to client, msg=111,     encrypted=300081,       char= o
Writing to client, msg=110,     encrypted=641211,       char= n
Writing to client, msg=100,     encrypted=470474,       char= d
Message sent!
Please enter a message to send (type '~' to quit):
Third
Writing to client, msg=84,      encrypted=399927,       char= T
Writing to client, msg=104,     encrypted=220567,       char= h
Writing to client, msg=105,     encrypted=681622,       char= i
Writing to client, msg=114,     encrypted=298194,       char= r
Writing to client, msg=100,     encrypted=470474,       char= d
Message sent!
Please enter a message to send (type '~' to quit):
Planet
Writing to client, msg=80,      encrypted=434896,       char= P
Writing to client, msg=108,     encrypted=253311,       char= l
Writing to client, msg=97,      encrypted=543527,       char= a
Writing to client, msg=110,     encrypted=641211,       char= n
Writing to client, msg=101,     encrypted=483757,       char= e
Writing to client, msg=116,     encrypted=497469,       char= t
Message sent!
Please enter a message to send (type '~' to quit):
Captain planet!
Writing to client, msg=67,      encrypted=616983,       char= C
Writing to client, msg=97,      encrypted=543527,       char= a
Writing to client, msg=112,     encrypted=526460,       char= p
Writing to client, msg=116,     encrypted=497469,       char= t
Writing to client, msg=97,      encrypted=543527,       char= a
Writing to client, msg=105,     encrypted=681622,       char= i
Writing to client, msg=110,     encrypted=641211,       char= n
Writing to client, msg=32,      encrypted=357146,       char=  
Writing to client, msg=112,     encrypted=526460,       char= p
Writing to client, msg=108,     encrypted=253311,       char= l
Writing to client, msg=97,      encrypted=543527,       char= a
Writing to client, msg=110,     encrypted=641211,       char= n
Writing to client, msg=101,     encrypted=483757,       char= e
Writing to client, msg=116,     encrypted=497469,       char= t
Writing to client, msg=33,      encrypted=619169,       char= !
Message sent!
Please enter a message to send (type '~' to quit):
From client:  encrypted: 8945   decrypted  79   O
From client:  encrypted: 8782   decrypted  107  k
From client:  encrypted: 9667   decrypted  97   a
From client:  encrypted: 8551   decrypted  121  y
From client:  encrypted: 1078   decrypted  44   ,
From client:  encrypted: 6292   decrypted  32    
From client:  encrypted: 553    decrypted  116  t
From client:  encrypted: 2992   decrypted  105  i
From client:  encrypted: 5893   decrypted  109  m
From client:  encrypted: 5441   decrypted  101  e
From client:  encrypted: 6292   decrypted  32    
From client:  encrypted: 553    decrypted  116  t
From client:  encrypted: 5557   decrypted  111  o
From client:  encrypted: 6292   decrypted  32    
From client:  encrypted: 9266   decrypted  113  q
From client:  encrypted: 8697   decrypted  117  u
From client:  encrypted: 2992   decrypted  105  i
From client:  encrypted: 553    decrypted  116  t
'Quit' message received from client.
Closing connection...
'read_from_client' thread exiting...

Please press 'ENTER' to proceed...


Waiting for new client...^C
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> 



------------------------------------------------------------------------------------------------------------------
RSA Chat (client): -----------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------


ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> ls      
client.cpp  crack_rsa      csapp.cpp  makefile    README.txt~   rsa_test      rsa_tools.cpp  server.cpp
client_rsa  crack_rsa.cpp  csapp.h    README.txt  rsa_chat.cpp  rsa_test.cpp  rsa_tools.h    server_rsa
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> client_rsa tepidity.seas.upenn.edu 8812

User Input:
        Host:   tepidity.seas.upenn.edu
        Port:   8812

Welcome to RSAchat client! 

Would you like your key values to be generated automatically? (y/n)
(Careful! If you enter your own, these values will not be checked for validity!)
y
Prime numbers are being randomly generated for you...
Done! Your selected primes are: A=23 and B=433
Your public key is (9467, 9959) and your private key is (5651, 9959)
Connecting to host...Done!
Sending request...
Sending public key (9467, 9959) to server.
Reading in server public key:
The server's public key is (237701, 705431)
Entering 'read from server' loop:
Please enter a message to send (type '~' to quit):
Message sent!
Please enter a message to send (type '~' to quit):
I am typing this first
Writing to server, char= I , msg=73, encrypted=9048
Writing to server, char=   , msg=32, encrypted=6292
Writing to server, char= a , msg=97, encrypted=9667
Writing to server, char= m , msg=109, encrypted=5893
Writing to server, char=   , msg=32, encrypted=6292
Writing to server, char= t , msg=116, encrypted=553
Writing to server, char= y , msg=121, encrypted=8551
Writing to server, char= p , msg=112, encrypted=2340
Writing to server, char= i , msg=105, encrypted=2992
Writing to server, char= n , msg=110, encrypted=6870
Writing to server, char= g , msg=103, encrypted=797
Writing to server, char=   , msg=32, encrypted=6292
Writing to server, char= t , msg=116, encrypted=553
Writing to server, char= h , msg=104, encrypted=8104
Writing to server, char= i , msg=105, encrypted=2992
Writing to server, char= s , msg=115, encrypted=2967
Writing to server, char=   , msg=32, encrypted=6292
Writing to server, char= f , msg=102, encrypted=4506
Writing to server, char= i , msg=105, encrypted=2992
Writing to server, char= r , msg=114, encrypted=413
Writing to server, char= s , msg=115, encrypted=2967
Writing to server, char= t , msg=116, encrypted=553
Message sent!
Please enter a message to send (type '~' to quit):
From server: encrypted=422700   decrypted=79    char= O
From server: encrypted=341337   decrypted=107   char= k
From server: encrypted=543527   decrypted=97    char= a
From server: encrypted=628716   decrypted=121   char= y
From server: encrypted=144310   decrypted=44    char= ,
From server: encrypted=357146   decrypted=32    char=  
From server: encrypted=641211   decrypted=110   char= n
From server: encrypted=300081   decrypted=111   char= o
From server: encrypted=382565   decrypted=119   char= w
From server: encrypted=357146   decrypted=32    char=  
From server: encrypted=167733   decrypted=73    char= I
From server: encrypted=357146   decrypted=32    char=  
From server: encrypted=543527   decrypted=97    char= a
From server: encrypted=32882    decrypted=109   char= m
From server: encrypted=357146   decrypted=32    char=  
From server: encrypted=497469   decrypted=116   char= t
From server: encrypted=628716   decrypted=121   char= y
From server: encrypted=526460   decrypted=112   char= p
From server: encrypted=681622   decrypted=105   char= i
From server: encrypted=641211   decrypted=110   char= n
From server: encrypted=227500   decrypted=103   char= g
From server: encrypted=357146   decrypted=32    char=  
From server: encrypted=497469   decrypted=116   char= t
From server: encrypted=220567   decrypted=104   char= h
From server: encrypted=681622   decrypted=105   char= i
From server: encrypted=647985   decrypted=115   char= s
From server: encrypted=357146   decrypted=32    char=  
From server: encrypted=647985   decrypted=115   char= s
From server: encrypted=483757   decrypted=101   char= e
From server: encrypted=74667    decrypted=99    char= c
From server: encrypted=300081   decrypted=111   char= o
From server: encrypted=641211   decrypted=110   char= n
From server: encrypted=470474   decrypted=100   char= d

From server: encrypted=399927   decrypted=84    char= T
From server: encrypted=220567   decrypted=104   char= h
From server: encrypted=681622   decrypted=105   char= i
From server: encrypted=298194   decrypted=114   char= r
From server: encrypted=470474   decrypted=100   char= d

From server: encrypted=434896   decrypted=80    char= P
From server: encrypted=253311   decrypted=108   char= l
From server: encrypted=543527   decrypted=97    char= a
From server: encrypted=641211   decrypted=110   char= n
From server: encrypted=483757   decrypted=101   char= e
From server: encrypted=497469   decrypted=116   char= t

From server: encrypted=616983   decrypted=67    char= C
From server: encrypted=543527   decrypted=97    char= a
From server: encrypted=526460   decrypted=112   char= p
From server: encrypted=497469   decrypted=116   char= t
From server: encrypted=543527   decrypted=97    char= a
From server: encrypted=681622   decrypted=105   char= i
From server: encrypted=641211   decrypted=110   char= n
From server: encrypted=357146   decrypted=32    char=  
From server: encrypted=526460   decrypted=112   char= p
From server: encrypted=253311   decrypted=108   char= l
From server: encrypted=543527   decrypted=97    char= a
From server: encrypted=641211   decrypted=110   char= n
From server: encrypted=483757   decrypted=101   char= e
From server: encrypted=497469   decrypted=116   char= t
From server: encrypted=619169   decrypted=33    char= !
Okay, time to quit
Writing to server, char= O , msg=79, encrypted=8945
Writing to server, char= k , msg=107, encrypted=8782
Writing to server, char= a , msg=97, encrypted=9667
Writing to server, char= y , msg=121, encrypted=8551
Writing to server, char= , , msg=44, encrypted=1078
Writing to server, char=   , msg=32, encrypted=6292
Writing to server, char= t , msg=116, encrypted=553
Writing to server, char= i , msg=105, encrypted=2992
Writing to server, char= m , msg=109, encrypted=5893
Writing to server, char= e , msg=101, encrypted=5441
Writing to server, char=   , msg=32, encrypted=6292
Writing to server, char= t , msg=116, encrypted=553
Writing to server, char= o , msg=111, encrypted=5557
Writing to server, char=   , msg=32, encrypted=6292
Writing to server, char= q , msg=113, encrypted=9266
Writing to server, char= u , msg=117, encrypted=8697
Writing to server, char= i , msg=105, encrypted=2992
Writing to server, char= t , msg=116, encrypted=553
Message sent!
Please enter a message to send (type '~' to quit):
~
User entered 'quit' message.
Writing 'quit' message to server...
Chat client exiting...
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6>


------------------------------------------------------------------------------------------------------------------
RSA Key Cracker: -------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------

ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> make crack_rsa     
g++ -g -lpthread  rsa_tools.cpp crack_rsa.cpp -o crack_rsa
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> ls
client.cpp  crack_rsa.cpp  csapp.h   README.txt   rsa_chat.cpp  rsa_tools.cpp  server.cpp
crack_rsa   csapp.cpp      makefile  README.txt~  rsa_test.cpp  rsa_tools.h
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> crack_rsa
Beginning key cracking sequence. 
Enter in public key value (e) and the corresponding (c) value:
4793 9271       
Found original primes.  a = 73  b = 127
Found decryption key 'd'.  d = 4889
Enter a single value (int) to decrypt, or the character 'q' to exit (or 't' to toggle these prompts): 
6355
        You entered: 6355
        This char decrypted to 89
        The letter is                   Y
Enter a single value (int) to decrypt, or the character 'q' to exit (or 't' to toggle these prompts): 
t
Found 't', toggling prompts...
6355
        Y
1333
        o
8077
        u
2683
         
1668
        h
2339
        a
7626
        v
8872
        e
2683
         
2728
        d
8872
        e
1053
        c
3230
        r
3527
        y
7451
        p
1001
        t
8872
        e
2728
        d
2683
         
4510
        m
3527
        y
2683
         
4510
        m
8872
        e
6946
        s
6946
        s
2339
        a
6372
        g
8872
        e
6185
        !
t
Found 't', toggling prompts...
Enter a single value (int) to decrypt, or the character 'q' to exit (or 't' to toggle these prompts): 
q
Found 'q', exiting...
ringlis@TEPIDITY:/mnt/castor/seas_home/r/ringlis/CIT_595/A6> 