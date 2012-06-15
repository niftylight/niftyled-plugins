
#ifdef DEBUG
DEBUG_FLAGS = -ggdb -O0
#endif

libdir = @libdir@/niftyled-plugins
INCLUDES = -I$(top_srcdir)


h_sources = niftylino.h niftylino_usb.h
c_sources = niftylino.c

lib_LTLIBRARIES=niftylino-hardware.la
niftylino_hardware_la_SOURCES=$(h_sources) $(c_sources)
niftylino_hardware_la_CFLAGS=-Wall $(niftyled_CFLAGS) $(libusb_CFLAGS) $(DEBUG_CFLAGS)
niftylino_hardware_la_LIBADD=$(niftyled_LIBS) $(libusb_LIBS)
niftylino_hardware_la_LDFLAGS= -module -avoid-version

