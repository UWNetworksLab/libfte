AC_INIT([libfte],[0.0.1])

AC_SUBST(gmpdir)
AC_ARG_WITH(gmp, 
            AS_HELP_STRING([--with-gmp=DIR],
                           [Specify path to the GMP directory]),
            [gmpdir="$withval"],
            AC_MSG_FAILURE([You must specify the path to the gmp directory])
           )

AC_PROG_CC
AC_PROG_CXX
AC_DEFUN([AC_PROG_AR], [AC_CHECK_TOOL(AR, ar, :)])
AC_PROG_AR
AC_LANG([C++])

AC_CHECK_FILE(third_party/gtest/Makefile,
              [],
              cd third_party/gtest && ./configure --disable-static --enable-shared && cd ../..)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
