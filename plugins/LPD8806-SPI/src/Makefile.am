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
	lpd8806-spi.h

# target library
lib_LTLIBRARIES = spi_lpd8806-hardware.la

# sources
spi_lpd8806_hardware_la_SOURCES = \
	lpd8806-spi.c

# cflags
spi_lpd8806_hardware_la_CFLAGS = \
	$(INCLUDE_DIRS) \
	$(niftyled_CFLAGS) \
	$(COMMON_CFLAGS_N) \
	$(DEBUG_CFLAGS)

# link in additional libraries
spi_lpd8806_hardware_la_LIBADD = \
	$(niftyled_LIBS) \
	$(COMMON_LIBS_N)

# linker flags
spi_lpd8806_hardware_la_LDFLAGS = \
	$(COMMON_LDFLAGS_N) \
	-no-allow-shlib-undefined \
	-module -avoid-version
