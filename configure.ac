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
    [echo "libusb found: compiling USB plugins" ; BUILD_USB_PLUGINS=true], \
    [echo "no libusb found: not compiling USB plugins" ; BUILD_USB_PLUGINS=false])

AC_SUBST(NL_PLUGIN_EXAMPLE)
AC_SUBST(NL_PLUGIN_NIFTYLINO)
AC_SUBST(NL_PLUGIN_ARDUINO_72XX)

AC_SUBST(libusb_CFLAGS)
AC_SUBST(libusb_LIBS)

# check for libartnet
PKG_CHECK_MODULES(libartnet, \
    [libartnet >= 1.0.6], \
    [echo "libartnet found: compiling artnet plugin" ; BUILD_ARTNET_PLUGINS=true], \
    [echo "no libartnet found: not compiling artnet plugin" ; BUILD_ARTNET_PLUGINS=false])



# --------------------------------
# decide about which plugins to compile
# --------------------------------


# dummy/example plugin
NL_PLUGIN_DUMMY=true

# arduino max72xx serial plugin
NL_PLUGIN_ARDUINO_72XX=true

# build USB plugins?
if test "$BUILD_USB_PLUGINS" = "true" ; then
    # niftylino USB adapter
    NL_PLUGIN_NIFTYLINO=true
fi


# build artnet plugins?
if test "$BUILD_ARTNET_PLUGINS" = "true" ; then
    # artnet plugin
    NL_PLUGIN_ARTNET=true
fi



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


# dummy/example plugin argument
NL_WANT_PLUGIN_DUMMY=true
AC_ARG_ENABLE(
	dummy_plugin,
	[  --enable-dummy-plugin         Build dummy plugin ],
	[ if test x$enableval = xfalse ; then NL_WANT_PLUGIN_DUMMY=false ; fi ])
AM_CONDITIONAL(NL_PLUGIN_DUMMY, test x$NL_WANT_PLUGIN_DUMMY = xtrue && test x$NL_PLUGIN_DUMMY = xtrue)

# arduino 72xx serial plugin argument
NL_WANT_PLUGIN_ARDUINO_72XX=true
AC_ARG_ENABLE(
	arduino_72xx_plugin,
	[  --enable-arduino_72xx-plugin         Build dummy plugin ],
	[ if test x$enableval = xfalse ; then NL_WANT_PLUGIN_ARDUINO_72XX=false ; fi ])
AM_CONDITIONAL(NL_PLUGIN_ARDUINO_72XX, test x$NL_WANT_PLUGIN_ARDUINO_72XX = xtrue && test x$NL_PLUGIN_ARDUINO_72XX = xtrue)

# niftylino plugin argument
NL_WANT_PLUGIN_NIFTYLINO=true
AC_ARG_ENABLE(
	niftylino_plugin,
	[  --enable-niftylino-plugin         Build niftylino plugin ],
	[ if test x$enableval = xfalse ; then NL_WANT_PLUGIN_NIFTYLINO=false ; fi ])
AM_CONDITIONAL(NL_PLUGIN_NIFTYLINO, test x$NL_WANT_PLUGIN_NIFTYLINO = xtrue && test x$NL_PLUGIN_NIFTYLINO = xtrue)

# libartnet plugin argument
NL_WANT_PLUGIN_ARTNET=true
AC_ARG_ENABLE(
	artnet_plugin,
	[  --enable-artnet-plugin         Build libartnet plugin ],
	[ if test x$enableval = xfalse ; then NL_WANT_PLUGIN_ARTNET=false ; fi ])
AM_CONDITIONAL(NL_PLUGIN_ARTNET, test x$NL_WANT_PLUGIN_ARTNET = xtrue && test x$NL_PLUGIN_ARTNET = xtrue)


       
# --------------------------------
# Build string with plugins to build
# --------------------------------
if test x$NL_WANT_PLUGIN_DUMMY = xtrue && test x$NL_PLUGIN_DUMMY = xtrue ; then BUILD_PLUGINS="dummy $BUILD_PLUGINS" ; fi
if test x$NL_WANT_PLUGIN_ARDUINO_72XX = xtrue && test x$NL_PLUGIN_ARDUINO_72XX = xtrue ; then BUILD_PLUGINS="arduino-max72xx $BUILD_PLUGINS" ; fi
if test x$NL_WANT_PLUGIN_NIFTYLINO = xtrue && test x$NL_PLUGIN_NIFTYLINO = xtrue ; then BUILD_PLUGINS="niftylino $BUILD_PLUGINS" ; fi
if test x$NL_WANT_PLUGIN_ARTNET = xtrue && test x$NL_PLUGIN_ARTNET = xtrue ; then BUILD_PLUGINS="artnet $BUILD_PLUGINS" ; fi



# --------------------------------
# Output
# --------------------------------
AM_CONFIG_HEADER(config.h)      
AC_CONFIG_FILES([ \
	Makefile \
	plugins/Makefile \
	plugins/example/Makefile \
	plugins/example/src/Makefile \
	plugins/example/tests/Makefile \
	plugins/arduino-max7219_max7221/Makefile \
	plugins/arduino-max7219_max7221/src/Makefile \
	plugins/arduino-max7219_max7221/tests/Makefile \
	plugins/artnet/Makefile \
	plugins/artnet/src/Makefile \
	plugins/artnet/tests/Makefile \
	plugins/niftylino/Makefile \
	plugins/niftylino/src/Makefile \
	plugins/niftylino/tests/Makefile])
AC_OUTPUT


echo "
Configure report:

    Package.....................:  ${PACKAGE_STRING}
    Description.................:  ${PACKAGE_DESCRIPTION}
    Revision....................:  ${PACKAGE_GIT_VERSION}
    URL.........................:  ${PACKAGE_URL}
    Bugreports..................:  ${PACKAGE_BUGREPORT}

    Install prefix..............:  ${prefix}
    C compiler..................:  ${CC}
    C++ compiler................:  ${CPP}
    System CFLAGS...............:  ${CFLAGS}
    System CXXFLAGS.............:  ${CXXFLAGS}
    System LDFLAGS..............:  ${LDFLAGS}
    
    Building plugins............:  ${BUILD_PLUGINS}
"
