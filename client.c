/****************************************************
 * Mark Rushmere
 * CS 372
 * Project 1
 * Description: This is the client prrogram that will 
 * connect to a socket that was created using server.cpp
 * It will send a request to the chat server and after establishing 
 * a connection be able to send messages of up to 500
 * characters.
* *************************************************/


#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>


// Function proototypes
int setSocket(int* portNo);
void connectServer();
void setHandles();
void sendMessage();
void receiveMessage();

// Global variables
char* serverName;
struct sockaddr_in serverAddr;
int portNo, sockNo;
char* sendBuff;
char* recBuff;
char* handle;
char* serverHandle;
char* quit = "quit/";




int main(int argc, char** argv) {

	// Check if the port number was provided
	if(argc != 3) {
		printf("Proper usage: client [hostname] [port number]\n");
		exit(1);
	}
	else {
		// Assign the hostname and port number
		serverName = argv[1];
		portNo = atoi(argv[2]);
	} 

	sockNo = setSocket(&portNo);
	
	
	connectServer();


	setHandles();
	

	// Allocate memory for the sending and receiving buffers
	sendBuff = (char*) malloc(512);
	recBuff = (char*) malloc(512);
	
	while(1) {

		sendMessage();
		receiveMessage();
	}
	// Close the socket
	close(sockNo);



	return 0;

}

int setSocket(int* portNo) {
	// Create the TCP socket
	int sockNo;
	sockNo = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Check for errors
	if(sockNo < 0) {
		printf("Error establishing socket\n");
		exit(1);
	}

	// Allocate memory for the struct containing the server info
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	
	inet_aton("127.0.0.1", (struct in_addr*)&serverAddr.sin_addr.s_addr);
	// htons(portNo) converts the port number
	serverAddr.sin_port = htons(*portNo);
	//printf("serverAddr set\n");

	return sockNo;
}

void connectServer() {
	// Connect to the socket
	if(connect(sockNo, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		printf("Error connecting to the server port: %d\n", portNo);
		exit(1);
	}

}

void setHandles() {
	// send the client's handle
	handle = (char*) malloc(10);
	printf("Please enter your handle (up to 10 characters)");
	fgets(handle, 10, stdin);
	strtok(handle, "\n");
	send(sockNo, handle, strlen(handle), 0);


	// Get the server's handle
	serverHandle = (char*) malloc(10);
	if(recv(sockNo, serverHandle, 10, 0) < 0) {
		printf("error getting the handle from the server");
	}
}


void sendMessage() {

	// Get the message from the user
	printf("%s> ", handle);
	fgets(sendBuff, 512, stdin);
	// send it
	send(sockNo, sendBuff, strlen(sendBuff), 0);
	memset(sendBuff, 0, 512);
}

void receiveMessage() {

	if(recv(sockNo, recBuff, 512, 0) < 0) {
		printf("Error receiving");
	}
	else {
		if(strcmp(quit, recBuff) == 0) {
			printf("server terminated chat");
			exit(0);
		}
		printf("%s> ", serverHandle);
		printf("%s", recBuff);
		memset(recBuff, 0, 512);
	}
} 