#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"
#include <unistd.h>
#include	<time.h>

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[]) {
	time_t	ticks;  //variable to hold date and time data
	char sendbuffer[BUFSIZE]; // Buffer for sending data to the client 
	char send_stars[50] = "**********";
	char send_spacing[50] = "          ";
	char send_text[50] = "Hi from Jay!";
	char recvbuffer[BUFSIZE];
	int numBytes = 0;

	if (argc != 2) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)", "<Server Port>");

	in_port_t servPort = atoi(argv[1]); // First arg:  local port

		// Create socket for incoming connections
	int servSock; // Socket descriptor for server
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithSystemMessage("socket() failed");

	// Construct local address structure
	struct sockaddr_in servAddr;                  // Local address
	memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
	servAddr.sin_family = AF_INET;                // IPv4 address family
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
	servAddr.sin_port = htons(servPort);          // Local port

	// Bind to the local address
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("bind() failed");

	// Mark the socket so it will listen for incoming connections
	if (listen(servSock, MAXPENDING) < 0)
		DieWithSystemMessage("listen() failed");

	for (;;) { // Infinite for loop; runs forever
    
    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) NULL, NULL);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");
    
    while ((numBytes = recv(clntSock, recvbuffer, BUFSIZE - 1, 0)) > 0) {
    recvbuffer[numBytes] = '\0';
    fputs(recvbuffer, stdout);    

    } // end while

    // clntSock is connected to a client!
    snprintf(sendbuffer, sizeof(sendbuffer), "%.24s\r\n", ctime(&ticks)); //Create data and time string in outgoing buffer
    ssize_t numBytesSent = send(clntSock, sendbuffer, strlen(sendbuffer), 0); //Send date and time string to the client 
    ssize_t numBytesSent_stars = send(clntSock, send_stars, strlen(send_stars), 0);
    ssize_t numBytesSent_spacing = send(clntSock, send_spacing, strlen(send_spacing), 0);
    ssize_t numBytesSent_text = send(clntSock, send_text, strlen(send_text), 0);
    ssize_t numBytesSent_spacing2 = send(clntSock, send_spacing, strlen(send_spacing), 0);
    ssize_t numButesSent_stars2 = send(clntSock, send_stars, strlen(send_stars), 0);  

    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");

	close(clntSock); // Close client socket

  }  //end infinite for loop 
  // NOT REACHED
}
