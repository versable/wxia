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
AC_ARG_WITH([wx-config],
	[AS_HELP_STRING([--with-wx-config=PATH], [Specify path to alternate wx-config])],
	[wx_config="$withval"], [wx_config="wx-config"])
AC_MSG_CHECKING([for wxWidgets])
libwx_CPPFLAGS=$($wx_config --cppflags $wxconfig_FLAGS 2>/dev/null)
ret=$?
AM_CONDITIONAL([HAVE_WXWIDGETS], [test "$ret" -eq 0])
if [[ "$ret" -eq 0 ]]; then
	AC_MSG_RESULT([yes])
	echo "wxWidgets CPPFLAGS... $libwx_CPPFLAGS"

	libwx_CXXFLAGS=$($wx_config --cppflags $wxconfig_FLAGS)
	echo "wxWidgets CXXFLAGS... $libwx_CXXFLAGS"

	libwx_LIBS=$($wx_config --libs $wxconfig_FLAGS)
	echo "wxWidgets LIBS... $libwx_LIBS"
	libwx_LIBS=$($wx_config --libs $wxconfig_FLAGS | sed -e 's/-Wl,--subsystem,windows//g' -e 's/-mwindows//g')
	echo "wxWidgets adjusted LIBS... $libwx_LIBS"
else
	AC_MSG_RESULT([no])
fi;
AC_SUBST([libwx_CPPFLAGS])
AC_SUBST([libwx_CXXFLAGS])
AC_SUBST([libwx_LIBS])

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
