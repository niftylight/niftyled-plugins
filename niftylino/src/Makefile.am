# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 

#ifdef NL_PLUGIN_NIFTYLINO


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
	


h_sources = niftylino.h niftylino_usb.h
c_sources = niftylino.c

lib_LTLIBRARIES=niftylino-hardware.la

niftylino_hardware_la_SOURCES=$(h_sources) $(c_sources)
niftylino_hardware_la_CFLAGS=-Wall $(niftyled_CFLAGS) $(libusb_CFLAGS) $(DEBUG_CFLAGS)
niftylino_hardware_la_LIBADD=$(niftyled_LIBS) $(libusb_LIBS)
niftylino_hardware_la_LDFLAGS= -module -avoid-version


#endif
