#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <algorithm>

#include <iostream>
#include <sstream>
#include <cstdio>
#include <fstream>

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
	// struct sockaddr_in addr;
	// addr.sin_family = AF_INET;
	// addr.sin_port = htons(40001);     // short, network byte order
	// addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
	// if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
	//   perror("bind");
	//   return 1;
	// }


	//go through each one of the URLs and decode it
	for(int i = 1; i < argc; i++) {

		// create a socket using TCP IP
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in serverAddr;


		HttpRequest request;
		string website = string(argv[i]);
		website = website.substr(7); //parse out the http:// portion, this is guaranteed to us
		//cout << website << endl;
		int slashpos = website.find("/");
		string host;
		if(slashpos < 0){
			host = website;
			request.setUrl("/");
			request.setHost(website);
		}
		else{
			//cout << "/" + website.substr(slashpos+1) << endl;
			//cout << website.substr(0, slashpos) << endl;
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
		cout << "Connecting to Domain: " + domain + " Port: " + port << endl;
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
		cout << "Connecting from: " << ipstr << ":" <<
		ntohs(clientAddr.sin_port) << std::endl;

		string input;
		char buf[1024] = {0};
		stringstream ss;


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

		string filename = request.getUrl();
		if(filename.find("/") == 0){
			filename = filename.substr(filename.find("/")+1);
		}
		while((int)filename.find("/") > 0){
			filename = filename.substr(filename.find("/")+1);
		}
		if(filename.compare("") == 0){
			filename = "index.html";
		}
		//cout << filename << endl;

		string bufstr = "";

		int bytes_read_prev_round = 0;

		/*String searches*/
		string notfoundstr = "404 Not Found";
		string badrequeststr = "400 Bad Request";
		string okstr = "200 OK";

		while(1){ //read entire HTTP message
			int bytes_read_this_round;
			bytes_read_this_round = read(sockfd, buf, sizeof(buf));
			if(bytes_read_this_round == -1){
				cerr << "Error: Could not read from socket" << endl;
				exit(1); 
			}

			bufstr += string(buf, bytes_read_this_round);

			if(bytes_read_this_round == 0){
				cout << "Successfully grabbed?" << endl;
				break;
			}
			bytes_read_prev_round = bytes_read_this_round;
		}

		//begin parsing http message stored in bufstr?
		string workingline;
		int headerlinecount = 0;
		ss << bufstr;
		fstream fs;
		bool onBody = false;
		while(!ss.eof()){ //UPDATE THIS TO TOKENIZE
			getline(ss, workingline);
			//workingline.erase(std::remove(workingline.begin(), workingline.end(), '\r'), workingline.end());

			cout << workingline << endl;

			if(headerlinecount == 0){
				if(workingline.find(notfoundstr) != -1){
					cerr << "Error: 404 Not Found" << endl;
					goto NEXTURL;
				}
				else if(workingline.find(badrequeststr) != -1){
					cerr << "Error: 400 Bad Request" << endl;
					goto NEXTURL;
				}
				else if(workingline.find(okstr) != -1){
					cout << "200 OK" << endl;
					headerlinecount++;
					continue;
				}
				else { //none of the three desired codes were found
					cerr << "Error: Bad Response Message" << endl;
					goto NEXTURL;
				}
			}
			else { //currently parsing rest of the header
				if(workingline.compare("\r") == 0) { //if it's the empty line
					onBody = true;
					break;
				}
				cout << workingline << endl;
			}
		}
		if(onBody == false) {//somehow reached end of header without ever getting to the body
			cerr << "Error: Bad Response Message, missing body" << endl;
		}

		fs.open(("./" + filename).c_str(), fstream::out);
		fs << ss.rdbuf();
		fs.close();
		
		NEXTURL:
		close(sockfd);

		cout << "Finished processing " + string(argv[i]) << endl;

	}

  return 0;
}