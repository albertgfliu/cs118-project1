/* C functionalities */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <error.h>

/* C networking functionalities */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

/* C++ functionalities */
#include <string>
#include <iostream>
#include <sstream>

/* User-defined functionalities */
#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

string hostname = "localhost";
string port = "4000";
string filedir = ".";

void 
childsig_handler(int i) {
	while(waitpid(-1, NULL, WNOHANG) > 0)
		continue;
}


int
serveHttpRequest(int sock) {
	char buf[1024] = {0};
	memset(buf, '\0', sizeof(buf));

	string bufstr = "";
	int bytes_read_prev_round = 0;
	
	//read entire HTTP message? problem is that read is blocking. need logic for scanning for /r/n/r/n
	int bytes_read_this_round;
	bytes_read_this_round = read(sock, buf, sizeof(buf));
	if(bytes_read_this_round == -1){
		cerr << "Error: Could not read from socket" << endl;
		exit(1); 
	}

	bufstr += string(buf, bytes_read_this_round);

	if(bytes_read_this_round == 0){
		cout << "Successfully grabbed?" << endl;
	}

	//bytes_read_prev_round = bytes_read_this_round;
	//cout << bytes_read_this_round << endl;

	cout << bufstr << endl;

	HttpRequest request;
	request.decode(bufstr);

	string requestedfilepath = request.getUrl();

	// cout << "You wanted: " + requestedfilepath << endl;

	//now get rid of the backslash in the requested file path, open it, and serve it as part of an httprequest

	if (write(sock, buf, sizeof(buf)) == -1) {
	  perror("send");
	  return 6;
	}
	return 0;
}

int
main(int argc, char *argv[]) 
{
	/* Parsing command line options */
	if(argc > 4){
		cerr << "usage: " + string(argv[0]) + "hostname port file-dir" << endl;
		exit(1);
	}

	for(int i = 1; i < argc; i++){
		switch(i){
			case 1: 
				hostname = argv[i];
				break;
			case 2:
				port = argv[i];
				break;
			case 3:
				filedir = argv[i];
				break;
		}
	}

	cout << "Serving from " + hostname + ":" + port + "/" + filedir << endl;


	/* Set up server */
 	int sockfd, clientsockfd;
 	struct sockaddr_in serveraddr, clientaddr;
 	struct sigaction childsigaction;

 	//create handler for child processes to reap and restart
	childsigaction.sa_handler = childsig_handler;
	sigemptyset(&childsigaction.sa_mask);
	childsigaction.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &childsigaction, NULL) < 0) {
		cerr << "Error: could not install child sigaction handler" << endl;
		exit(1);
	}

 	//create TCP socket
 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
 	if(sockfd < 0){
 		cerr << "Error: could not open socket" << endl;
 		exit(1);
 	}

	// allow others to reuse the address, NECESSARY IN CASE OF SIGKILL
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		cerr << "Error: could not set socket option" << endl;
		exit(1);
	}

 	//bind address to socket
 	serveraddr.sin_family = AF_INET;
  	serveraddr.sin_port = htons(stoi(port));
  	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //TEMPORARY, CONVERT USING DNS LOOKUP LATER
 	memset(serveraddr.sin_zero, '\0', sizeof(serveraddr.sin_zero));

	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
		cerr << "Error: could not bind" << endl;
		exit(2);
	}

	//begin listening
	if (listen(sockfd, 10) < 0) { //can queue up to 10 connections
		cerr << "Error: could not listen" << endl;
		exit(3);
	}

	pid_t pid;
 	socklen_t clientaddrsize = sizeof(clientaddr);
	while(1){ //accept connections and fork to child processes for concurrency
		clientsockfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientaddrsize);
		if(clientsockfd < 0){
			cerr << "Error: could not accept" << endl;
			exit(4);
		}

		pid = fork();
		if (pid < 0) {
			cerr << "Error: unsuccessful fork" << endl;
		}
		else if(pid == 0){ //child process
			close(sockfd);
			serveHttpRequest(clientsockfd);
			exit(0);
		}
		else { //pid > 0, parent process
			close(clientsockfd);
		} 
	}
}
