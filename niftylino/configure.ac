AC_INIT(libniftylino-hardware, 0.1, bugs@niftylight.de,, http://wiki.niftylight.de/niftylino-hardware)
AM_INIT_AUTOMAKE(libniftylino-hardware, 0.1, bugs@niftylight.de, http://wiki.niftylight.de/niftylino-hardware)
AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C])


# Test for new silent rules and enable only if they are available
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

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

PKG_CHECK_MODULES(libusb, [libusb], [], [AC_MSG_ERROR([You need libusb + development headers installed])])
AC_SUBST(libusb_CFLAGS)
AC_SUBST(libusb_LIBS)

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
