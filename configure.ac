AC_INIT([libfte],[0.0.1])

AC_CHECK_FILE(third_party/gtest-1.7.0/Makefile,
              [],
              cd third_party/gtest-1.7.0 && ./configure --enable-static --disable-shared && cd ../..)

AC_CHECK_FILE(third_party/gtest-1.7.0/Makefile,
              [],
              [AC_MSG_ERROR(Failed to configure gtest.)])

AC_CHECK_FILE(third_party/gmp-6.0.0/Makefile,
              [],
              cd third_party/gmp-6.0.0 && ./configure --disable-assembly --disable-static --enable-shared && cd ../..)

AC_CHECK_FILE(third_party/gmp-6.0.0/Makefile,
              [],
              [AC_MSG_ERROR(Failed to configure GMP.)])
              

AC_CONFIG_FILES([Makefile])

AC_PROG_CC
AC_PROG_CXX

AC_CHECK_HEADERS(iostream)
AC_CHECK_HEADERS(regex)

AC_OUTPUT
