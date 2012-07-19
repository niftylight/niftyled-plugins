# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 



# compile for debugging ?
if DEBUG
    OPT_CFLAGS = -O0 -g
else
    OPT_CFLAGS = -O2
endif


# path to install plugins
libdir = @libdir@/niftyled-plugins


# include dirs
INCLUDE_DIRS = \
	-I$(top_srcdir)



# target library
lib_LTLIBRARIES=arduino-max72xx-hardware.la

# sources
arduino_max72xx_hardware_la_SOURCES = \
	arduino_max72xx.c

# cflags
arduino_max72xx_hardware_la_CFLAGS = \
	$(INCLUDE_DIRS) $(niftyled_CFLAGS) $(OPT_CFLAGS)

# link in additional libraries
arduino_max72xx_hardware_la_LIBADD = \
	$(niftyled_LIBS)

# linker flags
arduino_max72xx_hardware_la_LDFLAGS = \
	-no-allow-shlib-undefined \
	-module -avoid-version
