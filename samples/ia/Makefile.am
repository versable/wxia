AM_CPPFLAGS = -I$(top_srcdir) $(all_includes) -I$(top_srcdir)/include
AM_CXXFLAGS = ${libwx_CXXFLAGS}

INSTALL_PROGRAM = @INSTALL@

noinst_PROGRAMS = iatest

iatest_SOURCES = iatest.cpp
iatest_LDADD = ${libwx_LIBS} -L${top_builddir}/src/ia -lwxia

noinst_HEADERS =

MAINTAINERCLEANFILES = Makefile.in
