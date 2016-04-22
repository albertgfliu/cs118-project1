#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <stdio.h>
#include <string>
#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

string hostname = "localhost";
string port = "4000";
string filedir = "/tmp";

int
main(int argc, char *argv[]) 
{
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0); //create a TCP socket
}
