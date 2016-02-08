/*************************************************************************************
 * Mark Rushmere
 * CS 372
 * Project 1
 * Decription: This program (chatserver) opens a sock and listens for incoming requests. 
 * If a connection request is received and accepted from chatclient, the chatclient 
 * and this program will be able to send and receive messages until a user on either end
 * sends a "\quit" message. When the chatclient disconnects, this program will continue 
 * listening for incoming requests until one is received, in which case the process will
 * repeat, or a SIGINT is received. 
 * ***********************************************************************************/


#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <string>


// Function prototypes
int setSocket(int *portNo);
void bindSocket();
void listenForReq();
int connectClient();
void getClientHandle();
int sendMessage();
int receiveMessage();


// Global variables
struct sockaddr_in clientAddr, serverAddr;
int portNo, clientSock, serverSock, isConnected;
socklen_t clientLen;
char* sendBuff;
char* recBuff;
char* clientHandle;
char* handle;
char* quit;


int main(int argc, char** argv) {

	// Check if the port number was provided
	if(argc != 2) {
		printf("Proper usage: chatserver [port number]");
		exit(1);
	}
	else {
		portNo = atoi(argv[1]);
	}

	// Set up the TCP socket
		serverSock = setSocket(&portNo);

		// Bind the socket
		bindSocket();

	while(1) {
		// Establish a socket that listens for incoming reqests
		listenForReq();


		// Accept a client request
		isConnected = connectClient();
		

		// allocate memory to send and receive messages
		sendBuff = (char*) malloc(512);
		recBuff = (char*) malloc(512);
		quit = (char*) malloc(10);
		strcpy(quit, "\\quit\n");


		getClientHandle();

		
		while(1) {
			if(receiveMessage() == 0) {
				break;
			}
			if(sendMessage() == 0) {
				break;
			}
		}

	}

	close(serverSock);


	return 0;
}


// Fucntion to set up the server TCP socket

int setSocket(int *portNo) {
	// Set up the TCP socket
	int serverSock;
	serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serverSock < 0) {
		printf("Error opening socket");
		exit(1);
	}

	// Allocate memory for the server address information
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	// htonl and htons converts ints (long and short) to network byte order	
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(*portNo);


	return serverSock;
}

void bindSocket() {
	// Bind the socket
	if(bind(serverSock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0) {
		printf("Error binding the socket\n");
		exit(1);
	}
}

void listenForReq() {
	// Establish a socket that listens for incoming reqests
	listen(serverSock, 1);
	printf("listening for connection on port: %d\n", portNo);
}

int connectClient() {
	// Accept a client request
	clientLen = sizeof(clientAddr);
	clientSock = accept(serverSock, (struct sockaddr*) &clientAddr, &clientLen);
	if(clientSock < 0) {
		printf("Error on accept");
		return 0;
	}
	else {
		return 1;
	}
}

void getClientHandle() {
	// first get the handle from the client
	clientHandle = (char*) malloc(10);
	if(recv(clientSock, clientHandle, 10, 0) < 0) {
		printf("Error reading from socket");
	}
	else {
		printf("Now chatting with: %s\n", clientHandle);
		// Send the client the server's handle
		handle = (char*) malloc(10);
		sprintf(handle, "mark");
		send(clientSock, handle, strlen(handle), 0);
	}
}

int sendMessage() {
	// Send a response
	printf("%s> ",  handle);
	// Clear sendBuff

	fgets(sendBuff, 512, stdin);
	if(send(clientSock, sendBuff, strlen(sendBuff), 0) < 0) {
		printf("Error sending message\n");
	}
	if(strcmp(sendBuff, quit) == 0) {
			printf("terminating session..\n");
			close(clientSock);
			//close(serverSock);
			return 0;
	}
	memset(sendBuff, 0, 512);
	return 1;	
}

int receiveMessage() {
	if(recv(clientSock, recBuff, 512, 0) < 0) {
		printf("Error reading from socket");
	}
	else {
		if(strcmp(recBuff, quit) == 0) {
			printf("client terminated session..\n");
			close(clientSock);
			//close(serverSock);
			return 0;
		}
		printf("%s> ", clientHandle);
		printf("%s", recBuff);

	}
	memset(recBuff, 0, 512);

	return 1;
}


