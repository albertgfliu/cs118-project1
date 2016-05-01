#include <string>
#include <iostream>
#include "HttpResponse.h"

using namespace std;

void
HttpResponse::setStatusCode(string statuscode)
{
	m_statuscode = statuscode;
}

void
HttpResponse::setHttpVersion(string httpversion)
{
	m_httpversion = httpversion;
}

string 
HttpResponse::getStatusCode(void)
{
	return m_statuscode;
}

string
HttpResponse::getHttpVersion(void)
{
	return m_httpversion;
}

HttpResponse::HttpResponse()
{
	m_statuscode = "";
	m_httpversion = "";
	m_statuscode = "";
}