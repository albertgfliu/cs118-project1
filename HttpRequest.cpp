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

string
HttpRequest::getHost(void)
{
	return m_host;
}

void
HttpRequest::setHttpVersion(string httpversion)
{
	m_httpversion = httpversion;
}

string
HttpRequest::getHttpVersion(void)
{
	return m_httpversion;
}

string
HttpRequest::encode()
{
	string s;
	s = m_method + " " + m_url + " " + m_httpversion + "\r\n";
	s += "Host: " + m_host + "\r\n";
	s += "\r\n";
	cout << s << endl;
	return s;
}

int
HttpRequest::decodeFirstLine(string firstline)
{
	char *workingstr = strdup(firstline.c_str());
	char *pch;
	pch = strtok(workingstr, " ");
	int i = 0;
	while(pch != NULL){
		//printf("%s\n", pch);
		string pchstr = string(pch);
		//cout << pchstr << endl;
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
				setUrl(pchstr);
				break;
			case 2: //HTTP case
				if(pchstr.compare("HTTP/1.0") == 0){
					setHttpVersion(pchstr);
				}
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