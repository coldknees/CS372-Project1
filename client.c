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



int main(int argc, char** argv) {


	// Declare variables
	char* serverName;
	struct sockaddr_in serverAddr;
	int portNo, sockNo, connectNo, n;
	char* sendBuff;
	char* recBuff;
	char* handle;



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
	// Create the TCP socket
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
	serverAddr.sin_port = htons(portNo);
	printf("serverAddr set\n");
	
	// Connect to the socket
	connectNo = connect(sockNo, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	printf("connection number: %d\n", connectNo);
	if(connectNo < 0) {
		printf("Error connecting to the server port: %d\n", portNo);
		exit(1);
	}




	/******** ADD ERROR CHECKING LATER ***********/
	// Get the client's handle
	handle = (char*) malloc(10);
	printf("Please enter your handle (up to 10 characters)");
	fgets(handle, 10, stdin);


	// Get the message from the user
	sendBuff = (char*) malloc(512);
	recBuff = (char*) malloc(512);
	printf("Enter your message: \n");

	fgets(sendBuff, 512, stdin);
	// send it
	send(sockNo, sendBuff, strlen(sendBuff), 0);
	//memset(sendBuff, 0, 512);

	// now receive
	if(recv(sockNo, recBuff, 512, 0) < 0) {
		printf("Error receiving");
	}
	else {
		printf("Received: %s\n", recBuff);
		//memset(recBuff, 0, 512);
	}

	// Close the socket
	close(sockNo);



	return 0;

}
