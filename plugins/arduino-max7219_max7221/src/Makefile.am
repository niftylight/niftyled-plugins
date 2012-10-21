# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 

include $(top_srcdir)/Makefile.global.am



# path to install plugins
libdir = @libdir@/niftyled-plugins


# include dirs
INCLUDE_DIRS = \
	-I$(top_srcdir)

EXTRA_DIST = arduino_max72xx.h

# target library
lib_LTLIBRARIES=arduino-max72xx-hardware.la

# sources
arduino_max72xx_hardware_la_SOURCES = \
	arduino_max72xx.c

# cflags
arduino_max72xx_hardware_la_CFLAGS = \
	$(INCLUDE_DIRS) $(niftyled_CFLAGS) \
	$(COMMON_CFLAGS_N) $(OPT_CFLAGS)

# link in additional libraries
arduino_max72xx_hardware_la_LIBADD = \
	$(niftyled_LIBS) \
	$(COMMON_LIBS_N)

# linker flags
arduino_max72xx_hardware_la_LDFLAGS = \
	$(COMMON_LDFLAGS_N) \
	-no-allow-shlib-undefined \
	-module -avoid-version
