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
lib_LTLIBRARIES=niftylino-hardware.la

# sources
niftylino_hardware_la_SOURCES = \
	niftylino.c

# cflags
niftylino_hardware_la_CFLAGS = \
	$(INCLUDE_DIRS) $(niftyled_CFLAGS) $(libusb_CFLAGS) $(OPT_CFLAGS)

# link in additional libraries
niftylino_hardware_la_LIBADD = \
	$(niftyled_LIBS) $(libusb_LIBS)

# linker flags
niftylino_hardware_la_LDFLAGS = \
	-no-allow-shlib-undefined \
	-module -avoid-version
