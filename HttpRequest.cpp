#include <string>
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

HttpRequest::HttpRequest(void)
{
}