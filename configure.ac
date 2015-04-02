dnl Process this file with autoconf to produce a configure script.
AC_INIT([wxia], [0.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE()

AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl Checks for programs.
LT_INIT

dnl figure out wx library name
changequote(<<, >>)
wx-config --cflags | grep DEBUG > /dev/null
if [ $? -ne 0 ] 
then
  wxlibname=`wx-config --prefix`/lib/libwx_gtk.so
else
  wxlibname=`wx-config --prefix`/lib/libwx_gtkd.so
fi
changequote([, ])
AC_SUBST(wxlibname)

dnl Is there a better way or place to do this?
CFLAGS="`wx-config --cflags`"
CXXFLAGS="`wx-config --cxxflags`"
CPPFLAGS="`wx-config --cppflags`"
#LIBS="`wx-config --libs`"

dnl Check for features

dnl Checks for libraries.
dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_DIRENT
dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
dnl Checks for library functions.
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(vsnprintf)

dnl output

AC_OUTPUT([
	Makefile
	src/Makefile
	src/ia/Makefile
	samples/Makefile
	samples/ia/Makefile
	include/Makefile
	include/wx/Makefile
	include/wx/ia/Makefile
	])
