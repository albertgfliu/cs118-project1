CPP ?= g++

DISTDIR = cs118-1-$(user)

SOURCES = HttpRequest.cpp HttpResponse.cpp main.cpp
OBJECTS = $(subst .cpp,.o,$(SOURCES))

DIST_SOURCES = \
	$(SOURCES) \
	Makefile

all: web-server

web-server: $(SOURCES)
	$(CPP) -o $(SOURCES)

dist: $(DISTDIR).tar.gz

$(DISTDIR).tar.gz: $(DIST_SOURCES) 
	rm -fr $(DISTDIR)
	tar -czf $@.tmp --transform='s,^,$(DISTDIR)/,' $(DIST_SOURCES)
	mv $@.tmp $@

clean:
	rm -fr *.o *~ *.bak *.tar.gz core *.core *.tmp web-server $(DISTDIR)

.PHONY: all dist check clean
