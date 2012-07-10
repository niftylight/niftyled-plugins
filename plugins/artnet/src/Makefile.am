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
lib_LTLIBRARIES=artnet-hardware.la

# sources
artnet_hardware_la_SOURCES = \
	artnet.c

# cflags
artnet_hardware_la_CFLAGS = \
	-Wall $(INCLUDE_DIRS) $(niftyled_CFLAGS) $(libartnet_CFLAGS) $(OPT_CFLAGS)

# link in additional libraries
artnet_hardware_la_LIBADD = \
	$(niftyled_LIBS) $(libartnet_LIBS)

# linker flags
artnet_hardware_la_LDFLAGS = \
	-Wall -no-undefined -no-allow-shlib-undefined \
	-module -avoid-version
