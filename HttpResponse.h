#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>

class HttpResponse
{
	private:
		std::string m_statuscode;
		std::string m_httpversion;
		std::string m_payload;

	public:
		void setStatusCode(std::string statuscode);
		std::string getStatusCode(void);
		void setHttpVersion(std::string httpversion);
		std::string getHttpVersion(void);
		HttpResponse();
};

#endif
