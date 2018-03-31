#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mattools], [0.2], [baiwenlei@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])

dnl check which system
AC_CANONICAL_HOST

case $host in
  *-*-darwin* )
  	MATINV_LIBS=""
  ;;
  * )
  	MATINV_LIBS="-lm"
  ;;
esac

AC_SUBST(MATINV_LIBS)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_YACC

AM_PROG_LEX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
dnl AC_SEARCH_LIBS([gsl_linalg_LU_decomp], [gsl], [], [AC_MSG_ERROR(can't find libgsl)])
dnl AC_SEARCH_LIBS([cblas_dtrsv], [gslcblas], [], [AC_MSG_ERROR(can't find libgslcblas)])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT


echo "
  ($PACKAGE_NAME) version $PACKAGE_VERSION
  Prefix.........: $prefix
  C Compiler.....: $CC $CFLAGS $CPPFLAGS 
  C++ Compiler...: $CXX $CXXFLAGS $CPPFLAGS
  Linker.........: $LD $LDFLAGS $LIBS
  host...........: $host
  MATINV_LIBS....: $MATINV_LIBS
"