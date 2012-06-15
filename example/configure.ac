AC_INIT(libdummy-hardware, 0.1, bugs@niftylight.de,, http://wiki.niftylight.de/dummy-hardware)
AM_INIT_AUTOMAKE(libdummy-hardware, 0.1, bugs@niftylight.de, http://wiki.niftylight.de/dummy-hardware)

AC_CONFIG_MACRO_DIR([m4])

# Test for new silent rules and enable only if they are available
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Test for target-specific stuff when cross-compiling
case "${host}" in
    *-mingw32*|*-winnt*)
        CFLAGS="${CFLAGS} -DWIN32"
    ;;
esac

RELEASE=0
LIBVERSION=0
AC_SUBST(RELEASE)
AC_SUBST(LIBVERSION)



# -----------------------------------------------
# Checks for programs.
# -----------------------------------------------
AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_LIBTOOL
AC_LANG_CPLUSPLUS

PKG_CHECK_MODULES(niftyled, [niftyled], [], [AC_MSG_ERROR([You need libniftyled + development headers installed])])
AC_SUBST(niftyled_CFLAGS)
AC_SUBST(niftyled_LIBS)

AM_CONFIG_HEADER(config.h)


# --------------------------------
# Arguments for configure-script
# --------------------------------

AC_ARG_ENABLE(
        debug,
	[  --disable-debug         Don't compile for debugging ],
	[ debug=false ],
	[ debug=true])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

AC_ARG_ENABLE(
	tests,
	[  --disable-tests         Don't run tests before install ],
	[ tests=false ],
	[ tests=true])
AM_CONDITIONAL(RUN_TESTS, test x$tests = xtrue)


        
        
# -----------------------------------------------
# Generates Makefile's, configuration files and scripts
# -----------------------------------------------
AC_CONFIG_FILES([Makefile \
	src/Makefile \
	tests/Makefile])
AC_OUTPUT
