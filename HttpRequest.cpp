#include <string>
#include <iostream>
#include "HttpRequest.h"

using namespace std;

void
HttpRequest::setUrl(string url)
{
	m_url = url;
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


HttpRequest::HttpRequest(void)
{
	m_method = "GET";
	m_httpversion = "HTTP/1.0";
}