AM_CPPFLAGS = -I. -I../../include/
AM_CXXFLAGS = ${libwx_CXXFLAGS}

lib_LTLIBRARIES = libwxia.la

noinst_HEADERS = 

libwxia_la_LDFLAGS = -release @VERSION@
libwxia_la_LIBADD  = ${libwx_LIBS}

libwxia_la_SOURCES = \
	ia.cpp \
	sane.cpp \
	iasane.cpp \
	iasanedlg.cpp
