lib_LTLIBRARIES = libomniengine.la

AM_CFLAGS = -DBOOST_ALL_DYN_LINK $(PTHREAD_CFLAGS) $(BOOST_CPPFLAGS) -DOMNIENGINE_EXPORTS -DPKGLIBDIR=\"$(pkglibdir)\" $(CFLAG_VISIBILITY)
AM_CPPFLAGS = -DBOOST_ALL_DYN_LINK $(PTHREAD_CFLAGS) $(BOOST_CPPFLAGS) -DOMNIENGINE_EXPORTS -DPKGLIBDIR=\"$(pkglibdir)\" $(CFLAG_VISIBILITY)
AM_LDFLAGS = -ldl

libomniengine_la_SOURCES = encoding.cpp Exception.cpp Registry.cpp Entity.cpp Resolver.cpp InstanceResolver.cpp Parser.cpp ParserSupport.cpp Weave.cpp WeaverAdapterAsio.cpp
libomniengine_la_LIBADD = $(BOOST_LOCALE_LIB) $(BOOST_ASIO_LIB) $(BOOST_LOG_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_THREAD_LIB) $(BOOST_SYSTEM)
