# libniftyled plugins configure.ac 
# v0.3 - (c) Daniel Hiepler (2010-2012)

# --------------------------------
#    initialization
# --------------------------------

AC_INIT([niftyled-plugins], m4_esyscmd([./version.sh --short]), [http://github.com/niftylight/niftyled-plugins/issues], [niftyled-plugins], http://wiki.niftylight.de/Hardware-plugins)
AC_COPYRIGHT([Copyright (C) 2006-2012 Daniel Hiepler])

AM_INIT_AUTOMAKE([color-tests dist-bzip2 parallel-tests silent-rules -Wall])
AC_CONFIG_MACRO_DIR([m4])


# --------------------------------
#    information on the package
# --------------------------------

# infos
PACKAGE_DESCRIPTION="libniftyled runtime loadabled plugins"
AC_SUBST([PACKAGE_DESCRIPTION], [${PACKAGE_DESCRIPTION}], [package description])
AC_DEFINE_UNQUOTED(PACKAGE_DESCRIPTION, "${PACKAGE_DESCRIPTION}", [package description])

# versioning (s. version.sh)
AC_SUBST([PACKAGE_MAJOR_VERSION], m4_esyscmd([./version.sh --major]), [major version])
AC_SUBST([PACKAGE_MINOR_VERSION], m4_esyscmd([./version.sh --minor]), [minor version])
AC_SUBST([PACKAGE_MICRO_VERSION], m4_esyscmd([./version.sh --micro]), [micro version])
AC_SUBST([PACKAGE_GIT_VERSION], m4_esyscmd([./version.sh --git]), [git version])



# -----------------------------------------------
# Checks for programs.
# -----------------------------------------------
LT_INIT
AM_PROG_CC_C_O
AM_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG([0.14])
if test -z "$PKG_CONFIG"; then
  AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/ or check your distribution.])
fi


# --------------------------------
#    checks for libraries
# --------------------------------
PKG_CHECK_MODULES(niftyled, [niftyled], [], [AC_MSG_ERROR([You need libniftyled + development headers installed])])
AC_SUBST(niftyled_CFLAGS)
AC_SUBST(niftyled_LIBS)

PKG_CHECK_MODULES(libusb, \
    [libusb], \
    [echo "libusb found: compiling USB plugins" ; NL_PLUGIN_NIFTYLINO=true], \
    [AC_MSG_ERROR([You need libusb + development headers installed]) ; NL_PLUGIN_NIFTYLINO=true])
AC_SUBST(NL_PLUGIN_EXAMPLE)
AC_SUBST(NL_PLUGIN_NIFTYLINO)
AC_SUBST(libusb_CFLAGS)
AC_SUBST(libusb_LIBS)


# --------------------------------
# Check for headers
# --------------------------------


# --------------------------------
# Check for typesv
# --------------------------------


# --------------------------------
# Check for structures
# --------------------------------


# --------------------------------
# Check compiler characteristics
# --------------------------------


# --------------------------------
# Check for functions
# --------------------------------


# --------------------------------
# Check for system-services
# --------------------------------


# --------------------------------
# Check for target-specific stuff 
# when cross-compiling
# --------------------------------
case "${host}" in
    *-mingw32*|*-winnt*)
        CFLAGS="${CFLAGS} -DWIN32"
    ;;
esac


# --------------------------------
#   compile process
# --------------------------------
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


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
	[  --disable-tests         Don't build tests for "make check" ],
	[ tests=false ],
	[ tests=true])
AM_CONDITIONAL(RUN_TESTS, test x$tests = xtrue)


AC_ARG_ENABLE(
	example-plugin,
	[  --enable-example-plugin         Build dummy plugin ],
	[ NL_PLUGIN_EXAMPLE=false ],
	[ NL_PLUGIN_EXAMPLE=true])
AM_CONDITIONAL(NL_PLUGIN_EXAMPLE, test x$example-plugin = xtrue)


AC_ARG_ENABLE(
	niftylino-plugin,
	[  --enable-niftylino-plugin         Build niftylino plugin ],
	[ NL_PLUGIN_NIFTYLINO=false ],
	[ NL_PLUGIN_NIFTYLINO=true])
AM_CONDITIONAL(NL_PLUGIN_NIFTYLINO, test x$niftylino-plugin = xtrue)

        

# --------------------------------
# Output
# --------------------------------
AM_CONFIG_HEADER(config.h)      
AC_CONFIG_FILES([ \
	Makefile \
	niftylino/Makefile \
	niftylino/src/Makefile \
	niftylino/tests/Makefile \
	example/Makefile \
	example/src/Makefile \
	example/tests/Makefile])
AC_OUTPUT
