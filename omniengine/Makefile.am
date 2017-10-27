lib_LTLIBRARIES = libomniengine.la

AM_CFLAGS = -DOMNIENGINE_EXPORTS -DPKGLIBDIR=\"$(pkglibdir)\" $(CFLAG_VISIBILITY)
AM_CPPFLAGS = -DOMNIENGINE_EXPORTS -DPKGLIBDIR=\"$(pkglibdir)\" $(CFLAG_VISIBILITY)

libomniengine_la_SOURCES = Registry.cpp Entity.cpp Resolver.cpp Instance.cpp Exception.cpp Parser.cpp ParserSupport.cpp
libomniengine_la_LDFLAGS = -ldl
