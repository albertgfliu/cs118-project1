#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <iostream>
#include <sstream>

#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

int
main(int argc, char *argv[])
{
	if (argc < 2){
		printf("Usage: web-client [URL] [URL]...\n");
		exit(1);
	}
	// create a socket using TCP IP
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// struct sockaddr_in addr;
	// addr.sin_family = AF_INET;
	// addr.sin_port = htons(40001);     // short, network byte order
	// addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
	// if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
	//   perror("bind");
	//   return 1;
	// }

	struct sockaddr_in serverAddr;

	//go through each one of the URLs and decode it
	for(int i = 1; i < argc; i++) {
		HttpRequest request;
		string website = string(argv[i]);
		website = website.substr(7); //parse out the http:// portion, this is guaranteed to us
		cout << website << endl;
		int slashpos = website.find("/");
		string host;
		if(slashpos < 0){
			host = website;
			request.setUrl("/");
			request.setHost(website);
		}
		else{
			cout << "/" + website.substr(slashpos+1) << endl;
			cout << website.substr(0, slashpos) << endl;
			request.setUrl("/" + website.substr(slashpos+1));
			host = website.substr(0,slashpos);
			request.setHost(host);
		}
		int colonpos = host.find(":");
		string port, domain;
		if(colonpos == -1){
			port = "80";
			domain = host;
		}
		else{
			port = host.substr(colonpos+1);
			domain = host.substr(0, colonpos);	
		}
		cout << "Port: " + port << endl;
		cout << "Domain: " + domain << endl;
		struct hostent *hp = gethostbyname(domain.c_str());

		string internet_address = inet_ntoa(*( struct in_addr*)(hp -> h_addr_list[0]));

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(stoi(port));     // short, network byte order
		serverAddr.sin_addr.s_addr = inet_addr(internet_address.c_str());
		memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

		string requeststr;
		requeststr = request.encode();
		cout << requeststr << endl;

		// connect to the server
		if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
			fprintf(stderr, "Error: could not connect to server\n");
			exit(2);
		}

		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);
		if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
			fprintf(stderr, "Error: could not get socket name\n");
			exit(3);
		}

		char ipstr[INET_ADDRSTRLEN] = {'\0'};
		inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
		cout << "Set up a connection from: " << ipstr << ":" <<
		ntohs(clientAddr.sin_port) << std::endl;

		std::string input;
		char buf[1024] = {0};
		std::stringstream ss;


		memset(buf, '\0', sizeof(buf));

		cout << "Sending: \n" + requeststr << endl;

		if (write(sockfd, requeststr.c_str(), requeststr.size()) == -1) {
			cerr << "Error: Could not write to socket" << endl;
			exit(1);
		}

		// std::cin >> input;
		// if (send(sockfd, input.c_str(), input.size(), 0) == -1) {
		//   perror("send");
		//   return 4;
		// }

		string workingstr = "";
		while(1){
			int bytes_read_this_round;
			bytes_read_this_round = read(sockfd, buf, sizeof(buf));
			if(bytes_read_this_round == -1){
				cerr << "Error: Could not read from socket" << endl;
				exit(1); 
			}
			ss << buf << std::endl;
			// std::cout << buf << std::endl;

			// char testbuf[1024] = {0};
			// ss.getline(testbuf, 1024);
			// cout << string(testbuf) << endl;

			//   perror("recv");
			//   return 5;

			// if (ss.str() == "close\n")
			//   break;

			// ss.str("");

		}

		close(sockfd);

	}

  return 0;
}