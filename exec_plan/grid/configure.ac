AC_INIT([grid_framework], [0.4], [ptab@google.com])
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([src/header.h])
AC_CONFIG_SRCDIR([src/Makefile.am])
AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_LANG([C++])
LT_INIT
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/utils/Makefile
  src/netio/Makefile
  src/node_environment/Makefile
  src/node_environment_test_app/Makefile
  src/grid_server/Makefile
  src/grid_server/proto/Makefile
  src/sender/Makefile
])
LDFLAGS=["$LDFLAGS -I/usr/local/include -L/usr/local/lib:/url/lib"]

ACX_PTHREAD
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
CC="$PTHREAD_CC"

LIBS="$PTHREAD_LIBS $LIBS"

AC_PATH_TOOL([PROTOC], [protoc])
if test -z "$PROTOBUF_INCLUDES"; then
	PROTOBUF_INCLUDES="."
fi
AC_SUBST(PROTOBUF_INCLUDES)
if test -z "$PROTOBUF_LIB"; then
  AC_CHECK_LIB([protobuf],[main],[],[
  	echo "protobuf (libprotobuf.a) required."
  	echo ""
  	echo "locate the library in your system (e.g. locate libprotobuf.a)"
  	echo "and try to add it to LDFLAGS variable: "
  	echo " LDFLAGS=\"-L/usr/local/lib\" ./configure"
  	exit -1
  ])
  PROTOBUF_LIBDIR="/usr/lib"
else
 PROTOBUF_LIBDIR=`dirname ${PROTOBUF_LIB}`
fi
CFLAGS="$CFLAGS $PTHREAD_CFLAGS $BOOST_CPPFLAGS"
CPPFLAGS="$CPPFLAGS $PTHREAD_CFLAGS $BOOST_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS $BOOST_CPPFLAGS"
CC="$PTHREAD_CC"
AX_BOOST_BASE([1.40])
AX_BOOST_THREAD
AX_BOOST_SYSTEM
AC_OUTPUT
