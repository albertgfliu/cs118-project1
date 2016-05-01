#include <string>
#include <string.h>
#include <cstring>
#include <iostream>
#include "HttpRequest.h"

using namespace std;

void
HttpRequest::setUrl(string url)
{
	m_url = url;
}

string
HttpRequest::getUrl(void)
{
	return m_url;
}

void
HttpRequest::setMethod(string method)
{
	m_method = method;
}

void
HttpRequest::setContentLength(string contentlength)
{
	m_contentlength = contentlength;
}

void
HttpRequest::setHost(string host)
{
	m_host = host;
}

void
HttpRequest::setHttpVersion(string httpversion)
{
	m_httpversion = httpversion;
}

string
HttpRequest::encode()
{
	string s;
	s = m_method + " " + m_url + " " + m_httpversion + "\r\n";
	s += "\r\n";
	cout << s << endl;
	return s;
}

int
HttpRequest::decode(string bufstr)
{
	char *workingstr = strdup(bufstr.c_str());
	char *pch;
	pch = strtok(workingstr, " ");
	int i = 0;
	while(pch != NULL){
		printf("%s\n", pch);
		string pchstr = string(pch);
		switch(i){
			case 0: //GET case
				if(pchstr.compare("GET") == 0){
					setMethod(pchstr);
				}
				else{
					return -1; //400 Bad Request
				}
				break;
			case 1: //url case
				if(pchstr.compare("/") == 0){
					return -2; //404 not found
				}
				setUrl(pchstr);
				break;
			case 2: //HTTP case
				setHttpVersion(pchstr);
				break;
		}
		pch = strtok(NULL, " ");
		i++;
	}
	return 0;
}

HttpRequest::HttpRequest(void)
{
	m_method = "GET";
	m_httpversion = "HTTP/1.0";
}