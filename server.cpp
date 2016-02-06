/*************************************************************************************
 * Mark Rushmere
 * CS 372
 * Project 1
 * Decription: This file implements the server functionality of the
 * server/client chat system. 
 *
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
int setSocket(int *porNo);
void bindSocket();
void listenForReq();
int connectClient();
void getClientHandle();
void sendMessage();
void receiveMessage();


// Global variables
struct sockaddr_in clientAddr, serverAddr;
int portNo, clientSock, serverSock, isConnected;
socklen_t clientLen;
char* sendBuff;
char* recBuff;
char* clientHandle;
std::string handle = "mark";


int main(int argc, char** argv) {

	// Check if the port number was provided
	if(argc != 2) {
		printf("Proper usage: server [port number]");
		exit(1);
	}
	else {
		portNo = atoi(argv[1]);
	}

	// Set up the TCP socket
	serverSock = setSocket(&portNo);

	// Bind the socket
	bindSocket();
	

	// Establish a socket that listens for incoming reqests
	listenForReq();


	// Accept a client request
	isConnected = connectClient();


	// first get the handle from the client
	clientHandle = (char*) malloc(10);
	if(recv(clientSock, clientHandle, 10, 0) < 0) {
		printf("Error reading from socket");
	}
	else {
		printf("Now chatting with: %s", clientHandle);
	}
	// allocate memory to send and receive messages
	sendBuff = (char*) malloc(512);
	recBuff = (char*) malloc(512);

	if(recv(clientSock, recBuff, 512, 0) < 0) {
		printf("Error reading from socket");
	}
	else {
		printf("Here is the message: %s", recBuff);
		//memset(recBuff, 0, 512);
	}

	// Send a response
	printf("Enter your message: \n");
	// Clear sendBuff
	//memset(sendBuff, 0, 512);
	fgets(sendBuff, 512, stdin);
	if(send(clientSock, sendBuff, strlen(sendBuff), 0) < 0) {
		printf("Error sending message\n");
	}


	// Close the sockets
	close(clientSock);
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


void receiveMessage() {

}