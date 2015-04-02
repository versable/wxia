AM_CPPFLAGS = -I$(top_srcdir) $(all_includes) -I$(top_srcdir)/include
LIBS = `wx-config --libs`
LDADD = -L$(top_srcdir)/src/ia/ -lwxia

INSTALL_PROGRAM = @INSTALL@

noinst_PROGRAMS = iatest

iatest_SOURCES = iatest.cpp

noinst_HEADERS =

MAINTAINERCLEANFILES = Makefile.in
