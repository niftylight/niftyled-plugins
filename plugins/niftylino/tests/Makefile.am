# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 

include $(top_srcdir)/Makefile.global.am


my_cflags = -I$(top_srcdir)/src -I. $(niftyled_CFLAGS) $(OPT_CFLAGS) $(COMMON_CFLAGS_N)
my_libflags = $(niftyled_LIBS) $(COMMON_LIBS_N)
my_ldflags = $(COMMON_LDFLAGS_N)


check_PROGRAMS =

# test-target
TESTS = $(check_PROGRAMS)
TESTS_ENVIRONMENT = "$(top_srcdir)/tests/tests.env"
EXTRA_DIST = tests.env
