
#ifdef DEBUG
DEBUG_FLAGS = -ggdb -O0
#endif

libdir = @libdir@/niftyled-plugins
INCLUDES = -I$(top_srcdir)


h_sources = dummy.h
c_sources = dummy.c

lib_LTLIBRARIES=dummy-hardware.la
dummy_hardware_la_SOURCES=$(h_sources) $(c_sources)
dummy_hardware_la_CFLAGS=-Wall $(niftyled_CFLAGS) $(libusb_CFLAGS) $(DEBUG_CFLAGS)
dummy_hardware_la_LIBADD=$(niftyled_LIBS) $(libusb_LIBS)
dummy_hardware_la_LDFLAGS= -module -avoid-version

