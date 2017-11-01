lib_LTLIBRARIES = libomniengine.la

AM_CFLAGS = -DOMNIENGINE_EXPORTS -DPKGLIBDIR=\"$(pkglibdir)\" $(CFLAG_VISIBILITY)
AM_CPPFLAGS = -DOMNIENGINE_EXPORTS -DPKGLIBDIR=\"$(pkglibdir)\" $(CFLAG_VISIBILITY)
AM_LDFLAGS = -ldl

libomniengine_la_SOURCES = Exception.cpp Registry.cpp Entity.cpp Resolver.cpp InstanceResolver.cpp Parser.cpp ParserSupport.cpp Weave.cpp WeaverAdapterAsio.cpp
