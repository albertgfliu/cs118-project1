#include <string>

#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

int
main(int argc, char *argv[]) 
{
	HttpRequest request;
	request.setUrl("asdf\r\n");
}
