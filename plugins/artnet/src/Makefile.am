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


# target library
lib_LTLIBRARIES=udp_artnet-hardware.la

# sources
udp_artnet_hardware_la_SOURCES = \
	artnet.c

# cflags
udp_artnet_hardware_la_CFLAGS = \
	$(INCLUDE_DIRS) \
	$(niftyled_CFLAGS) \
	$(artnet_CFLAGS) \
	$(COMMON_CFLAGS_N) \
	$(DEBUG_CFLAGS)

# link in additional libraries
udp_artnet_hardware_la_LIBADD = \
	$(niftyled_LIBS) \
	$(artnet_LIBS) \
	$(COMMON_LIBS_N)

# linker flags
udp_artnet_hardware_la_LDFLAGS = \
	$(COMMON_LDFLAGS_N) \
	-no-allow-shlib-undefined \
	-module -avoid-version
