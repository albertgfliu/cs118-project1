#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include <stdint.h>

class HttpRequest
{
	private:
		std::string m_url;
		std::string m_method;
		std::string m_contentlength;
		std::string m_host;
		std::string m_httpversion;
	public:
		void setUrl(std::string url);
		std::string getUrl(void);
		void setMethod(std::string method);
		void setContentLength(std::string contentlength);
		void setHost(std::string host);
		std::string getHost(void);
		void setHttpVersion(std::string httpversion);
		std::string getHttpVersion(void);
		std::string encode();
		int decodeFirstLine(std::string firstline);

		HttpRequest();
};

#endif