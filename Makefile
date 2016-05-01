CPP = g++

CPPFLAGS = -std=c++11

USERID = 904167727

DISTDIR = $(USERID)

HTTPSOURCES = HttpRequest.cpp HttpResponse.cpp
HTTPHEADERS = HttpRequest.h HttpResponse.h
WEBSERVERSOURCES = web-server.cpp
WEBCLIENTSOURCES = web-client.cpp
SOURCES = $(HTTPHEADERS) $(HTTPSOURCES) $(WEBSERVERSOURCES) $(WEBCLIENTSOURCES)

HTTPOBJECTS = $(subst .cpp,.o,$(HTTPSOURCES))
WEBSERVEROBJECTS = $(subst .cpp,.o,$(WEBSERVERSOURCES))
WEBCLIENTOBJECTS = $(subst .cpp,.o,$(WEBCLIENTSOURCES))

DIST_SOURCES = \
	$(SOURCES) \
	Makefile


all: web-server web-client

web-server: $(WEBSERVERSOURCES) $(HTTPSOURCES) $(HTTPHEADERS)
	$(CPP) $(CPPFLAGS) $(WEBSERVERSOURCES) $(HTTPSOURCES) -o web-server

web-client: $(WEBCLIENTSOURCES) $(HTTPSOURCES) $(HTTPHEADERS)
	$(CPP) $(CPPFLAGS) $(WEBCLIENTSOURCES) $(HTTPSOURCES) -o web-client

tarball: $(USERID).tar.gz

$(USERID).tar.gz: $(DIST_SOURCES) 
	rm -fr $(DISTDIR)
	tar -czf $@.tmp --transform='s,^,$(DISTDIR)/,' $(DIST_SOURCES)
	mv $@.tmp $@

clean:
	rm -fr *.o *~ *.bak *.tar.gz core *.core *.tmp web-server web-client $(DISTDIR)

remake: clean all

.PHONY: all dist check clean
