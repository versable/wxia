AM_CPPFLAGS = -I${top_srcdir}/include
AM_CXXFLAGS = ${libwx_CXXFLAGS}

lib_LTLIBRARIES = libwxia.la

libwxia_la_LDFLAGS = -release @VERSION@
libwxia_la_LIBADD  = ${libwx_LIBS}

libwxia_la_SOURCES = \
	ia.cpp \
	sane.cpp \
	iasane.cpp \
	iasanedlg.cpp
