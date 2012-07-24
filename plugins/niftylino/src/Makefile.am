# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 

include $(top_srcdir)/Makefile.global.am



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
	$(INCLUDE_DIRS) $(niftyled_CFLAGS) $(libusb_CFLAGS) \
	$(COMMON_CFLAGS_N) $(OPT_CFLAGS)

# link in additional libraries
niftylino_hardware_la_LIBADD = \
	$(niftyled_LIBS) $(libusb_LIBS) \
	$(COMMON_LIBS_N)

# linker flags
niftylino_hardware_la_LDFLAGS = \
	$(COMMON_LDFLAGS_N) \
	-no-allow-shlib-undefined \
	-module -avoid-version
