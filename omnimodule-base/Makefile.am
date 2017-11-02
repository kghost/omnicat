pkglib_LTLIBRARIES = omnimodule-base.la

AM_CFLAGS = -DBOOST_ALL_DYN_LINK $(PTHREAD_CFLAGS) $(BOOST_CPPFLAGS) $(CFLAG_VISIBILITY)
AM_CPPFLAGS = -DBOOST_ALL_DYN_LINK $(PTHREAD_CFLAGS) $(BOOST_CPPFLAGS) $(CFLAG_VISIBILITY)
AM_LDFLAGS = -module -avoid-version -shared

omnimodule_base_la_SOURCES = Module.cpp EntityTcpListener.cpp EntityTcpResolver.cpp FactoryTcpListener.cpp FactoryTcpResolver.cpp InstanceTcpListener.cpp InstanceTcpResolver.cpp
