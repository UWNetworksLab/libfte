AC_INIT([libfte],[0.0.1])

AC_CONFIG_MACRO_DIR([m4])

AC_SUBST(gmpdir)
AC_ARG_WITH(gmp, 
            AS_HELP_STRING([--with-gmp=DIR],
                           [The path to your GMP directory]),
            [gmpdir="$withval"],
            [gmpdir="/usr/local"]
           )

AC_PROG_CC
AC_PROG_CXX
AC_DEFUN([AC_PROG_AR], [AC_CHECK_TOOL(AR, ar, :)])
AC_PROG_AR
AC_LANG([C++])

AX_CXX_COMPILE_STDCXX_11
if [test "x$HAVE_CXX11" = "x1"]; then
  CXXFLAGS="$CXXFLAGS -DHAVE_CXX11"
fi

AC_CHECK_FILE(third_party/gtest/Makefile,
              [],
              cd third_party/gtest && ./configure --disable-shared --enable-static && cd ../..)

AC_CHECK_HEADER([gmpxx.h], ,[AC_MSG_ERROR(
[Couldn't find gmpxx.h...try downloading the source from your package manager.] )])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
