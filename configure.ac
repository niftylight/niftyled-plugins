# libniftyled plugins configure.ac
# v0.4 - (c) Daniel Hiepler (2010-2012)


# --------------------------------
#    initialization
# --------------------------------

AC_INIT([niftyled-plugins], m4_esyscmd([./version --short]), [http://github.com/niftylight/niftyled-plugins/issues], [niftyled-plugins], http://wiki.niftylight.de/Hardware-plugins)
AC_COPYRIGHT([Copyright (C) 2006-2012 Daniel Hiepler])

AM_INIT_AUTOMAKE([color-tests dist-bzip2 parallel-tests silent-rules -Wno-portability -Wall])
AC_CONFIG_MACRO_DIR([m4])


# --------------------------------
#    information on the package
# --------------------------------

# infos
PACKAGE_DESCRIPTION="libniftyled runtime loadabled plugins"
PACKAGE_DESCRIPTION_LONG="libniftyled runtime loadabled plugins that provide support for various lighting hardware interfaces"

AC_SUBST([PACKAGE_DESCRIPTION], [${PACKAGE_DESCRIPTION}], [package description])
AC_SUBST([PACKAGE_DESCRIPTION_LONG], [${PACKAGE_DESCRIPTION_LONG}], [long package description])
AC_DEFINE_UNQUOTED(PACKAGE_DESCRIPTION, "${PACKAGE_DESCRIPTION}", [package description])
AC_DEFINE_UNQUOTED(PACKAGE_DESCRIPTION_LONG, "${PACKAGE_DESCRIPTION_LONG}", [long package description])

# versioning (s. version script)
AC_SUBST([PACKAGE_MAJOR_VERSION], m4_esyscmd([./version --major]), [major version])
AC_SUBST([PACKAGE_MINOR_VERSION], m4_esyscmd([./version --minor]), [minor version])
AC_SUBST([PACKAGE_MICRO_VERSION], m4_esyscmd([./version --micro]), [micro version])
AC_SUBST([PACKAGE_GIT_VERSION], m4_esyscmd([./version --git]), [git version])



# -----------------------------------------------
# Checks for programs.
# -----------------------------------------------
LT_INIT
AC_PROG_CC_STDC
AM_PROG_CC_C_O
AM_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG([0.14])
if test -z "${PKG_CONFIG}"; then
  AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/ or check your distribution.])
fi


# --------------------------------
#    checks for libraries
# --------------------------------
PKG_CHECK_MODULES(niftyled, [niftyled], [], [AC_MSG_ERROR([You need libniftyled + development headers installed])])
AC_SUBST(niftyled_CFLAGS)
AC_SUBST(niftyled_LIBS)

PKG_CHECK_MODULES(usb, [libusb], [HAVE_USB=1], [HAVE_USB=0])
AC_SUBST(usb_CFLAGS)
AC_SUBST(usb_LIBS)

# check for libartnet
PKG_CHECK_MODULES(artnet, [libartnet >= 1.0.6], [HAVE_ARTNET=1], [HAVE_ARTNET=0])
AC_SUBST(artnet_CFLAGS)
AC_SUBST(artnet_LIBS)



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
#   compile process
# --------------------------------
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


# --------------------------------
# Arguments for configure-script
# --------------------------------
AC_ARG_ENABLE(
        debug,
		AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
		[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
		esac],
		[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)


# always build dummy/example plugin
NL_WANT_PLUGIN_DUMMY=true
AM_CONDITIONAL(NL_PLUGIN_DUMMY, test x$NL_WANT_PLUGIN_DUMMY = xtrue)

# LPD8806 SPI plugin argument
AC_ARG_ENABLE(
	plugin-lpd8806-spi,
	AS_HELP_STRING([--enable-plugin-lpd8806-spi], [Build LPD8806 SPI plugin]),
	[ if test x$enableval = xno ; then NL_WANT_PLUGIN_LPD8806_SPI=false ; else NL_WANT_PLUGIN_LPD8806_SPI=true ; fi ],
	[NL_WANT_PLUGIN_LPD8806_SPI=true])
AM_CONDITIONAL(NL_PLUGIN_LPD8806_SPI, test x$NL_WANT_PLUGIN_LPD8806_SPI = xtrue)

# arduino 72xx serial plugin argument
AC_ARG_ENABLE(
	plugin-arduino-72xx,
	AS_HELP_STRING([--enable-plugin-arduino-72xx], [Build arduino-72xx plugin]),
	[ if test x$enableval = xno ; then NL_WANT_PLUGIN_ARDUINO_72XX=false ; else NL_WANT_PLUGIN_ARDUINO_72XX=true ; fi ],
	[NL_WANT_PLUGIN_ARDUINO_72XX=true])
AM_CONDITIONAL(NL_PLUGIN_ARDUINO_72XX, test x$NL_WANT_PLUGIN_ARDUINO_72XX = xtrue)

# niftylino plugin argument
AC_ARG_ENABLE(
	plugin-niftylino,
	AS_HELP_STRING([--enable-plugin-niftylino], [Build niftylino USB controller plugin]),
	[ if test x$enableval = xno ; then NL_WANT_PLUGIN_NIFTYLINO=false ; else if test $HAVE_USB -eq 1 ; then NL_WANT_PLUGIN_NIFTYLINO=true ; else AC_MSG_ERROR([Build of niftylinio USB plugin requested but libusb headers not found.]) ; fi ; fi ],
	[NL_WANT_PLUGIN_NIFTYLINO=true])
AM_CONDITIONAL([NL_PLUGIN_NIFTYLINO], test x$NL_WANT_PLUGIN_NIFTYLINO = xtrue && test $HAVE_USB -eq 1)

# libartnet plugin argument
AC_ARG_ENABLE(
	plugin-artnet,
	AS_HELP_STRING([--enable-plugin-artnet], [Build libartnet plugin]),
	[ if test x$enableval = xno ; then NL_WANT_PLUGIN_ARTNET=false ; else if test $HAVE_USB -eq 1 ; then NL_WANT_PLUGIN_ARTNET=true ; else AC_MSG_ERROR([Build of artnet plugin requested but libartnet headers not found.]) ; fi ; fi ],
	[NL_WANT_PLUGIN_ARTNET=true])
AM_CONDITIONAL(NL_PLUGIN_ARTNET, test x$NL_WANT_PLUGIN_ARTNET = xtrue && test $HAVE_ARTNET -eq 1)



# --------------------------------
# Build string with plugins to build
# --------------------------------
if test "x$NL_WANT_PLUGIN_DUMMY" = "xtrue" ; then BUILD_PLUGINS="dummy $BUILD_PLUGINS" ; fi
if test "x$NL_WANT_PLUGIN_ARDUINO_72XX" = "xtrue" ; then BUILD_PLUGINS="arduino-max72xx $BUILD_PLUGINS" ; fi
if test "x$NL_WANT_PLUGIN_NIFTYLINO" = "xtrue" && test $HAVE_USB -eq 1 ; then BUILD_PLUGINS="niftylino $BUILD_PLUGINS" ; fi
if test "x$NL_WANT_PLUGIN_ARTNET" = "xtrue" && test $HAVE_ARTNET -eq 1 ; then BUILD_PLUGINS="artnet $BUILD_PLUGINS" ; fi
if test "x$NL_WANT_PLUGIN_LPD8806_SPI" = "xtrue" ; then BUILD_PLUGINS="LPD8806-SPI $BUILD_PLUGINS" ; fi


# --------------------------------
# Output
# --------------------------------
AM_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([
	Makefile
	plugins/Makefile
	plugins/example/Makefile
	plugins/example/src/Makefile
	plugins/example/tests/Makefile
	plugins/arduino-max7219_max7221/Makefile
	plugins/arduino-max7219_max7221/src/Makefile
	plugins/arduino-max7219_max7221/tests/Makefile
	plugins/artnet/Makefile
	plugins/artnet/src/Makefile
	plugins/artnet/tests/Makefile
	plugins/niftylino/Makefile
	plugins/niftylino/src/Makefile
	plugins/niftylino/tests/Makefile
	plugins/LPD8806-SPI/Makefile
	plugins/LPD8806-SPI/src/Makefile
	plugins/LPD8806-SPI/tests/Makefile
	debian/control.tmpl
	gentoo/media-gfx/niftyled-plugins/niftyled-plugins-9999.ebuild])
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
