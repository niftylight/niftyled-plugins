#############
# libniftyled plugins Makefile.am
# v0.4 - Daniel Hiepler <daniel@niftylight.de>


include $(top_srcdir)/Makefile.global.am



# path to install plugins
libdir = @libdir@/niftyled-plugins


# include dirs
INCLUDE_DIRS = \
	-I$(top_srcdir) \
	-I$(top_builddir)

# files to include in archive
EXTRA_DIST = \
	niftylino.h \
	niftylino_usb.h

# target library
lib_LTLIBRARIES=usb_niftylino-hardware.la

# sources
usb_niftylino_hardware_la_SOURCES = \
	niftylino.c

# cflags
usb_niftylino_hardware_la_CFLAGS = \
	$(INCLUDE_DIRS) \
	$(niftyled_CFLAGS) \
	$(usb_CFLAGS) \
	$(COMMON_CFLAGS_N) \
	$(DEBUG_CFLAGS)

# link in additional libraries
usb_niftylino_hardware_la_LIBADD = \
	$(niftyled_LIBS) \
	$(usb_LIBS) \
	$(COMMON_LIBS_N)

# linker flags
usb_niftylino_hardware_la_LDFLAGS = \
	$(COMMON_LDFLAGS_N) \
	-no-allow-shlib-undefined \
	-module -avoid-version
