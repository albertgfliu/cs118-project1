#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>

class HttpRequest
{
	private:
		std::string m_url;
		std::string m_method;
		std::string m_contentlength;
		std::string m_host;
	public:
		void setUrl(std::string url);
		void setMethod(std::string method);
		void setContentLength(std::string contentlength);
		void setHost(std::string host);
		HttpRequest();
};

#endif