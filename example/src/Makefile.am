# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 

#ifdef NL_PLUGIN_EXAMPLE


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
lib_LTLIBRARIES=dummy-hardware.la

# sources
lib@PACKAGE@_la_SOURCES = dummy.c

# cflags
lib@PACKAGE@_la_CFLAGS = \
	-Wall $(INCLUDE_DIRS) $(niftyled_CFLAGS) $(libusb_CFLAGS) $(OPT_CFLAGS)

# link in additional libraries
lib@PACKAGE@_la_LIBADD = \
	$(niftyled_LIBS) $(libusb_LIBS)

# linker flags
lib@PACKAGE@_la_LDFLAGS = \
	-Wall -no-undefined -no-allow-shlib-undefined \
	-module \
	-version-info @PACKAGE_API_CURRENT@:@PACKAGE_API_REVISION@:@PACKAGE_API_AGE@ \
	-release @PACKAGE_MAJOR_VERSION@.@PACKAGE_MINOR_VERSION@.@PACKAGE_MICRO_VERSION@ \

#endif
