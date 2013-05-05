CC= g++
CFLAGS= -g -lpthread 
SRC = server.cpp rsa_tools.cpp csapp.cpp
HDR = rsa_tools.h csapp.h
EXEC = server

all: server client test crack_rsa

test: $(HDR) rsa_test.cpp rsa_tools.cpp 
	$(CC) $(CFLAGS) rsa_tools.cpp rsa_test.cpp -o rsa_test

server: $(HDR) server.cpp rsa_tools.cpp csapp.cpp
	$(CC) $(CFLAGS) rsa_tools.cpp csapp.cpp server.cpp -o server_rsa

client: $(HDR) client.cpp rsa_tools.cpp csapp.cpp
	$(CC) $(CFLAGS) rsa_tools.cpp csapp.cpp client.cpp -o client_rsa

crack_rsa: $(HDR) crack_rsa.cpp rsa_tools.cpp
	$(CC) $(CFLAGS) rsa_tools.cpp crack_rsa.cpp -o crack_rsa

clean:
	rm crack_rsa
	rm rsa_test
	rm server_rsa
	rm client_rsa

run: $(EXEC)

