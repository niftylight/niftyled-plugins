# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 

include $(top_srcdir)/Makefile.global.am


tests_CFLAGS_PRIV = -g -O0 $(niftyled_CFLAGS) $(OPT_CFLAGS) $(COMMON_CFLAGS_N)
tests_LDFLAGS_PRIV = $(COMMON_LDFLAGS_N)
tests_LIBADD_PRIV = $(niftyled_LIBS) $(COMMON_LIBS_N)


# test-target
check_PROGRAMS = generic
TESTS = $(check_PROGRAMS)
TESTS_ENVIRONMENT = ./tests.env
EXTRA_DIST = tests.env

generic_SOURCES = generic.c
generic_CFLAGS = $(tests_CFLAGS_PRIV)
generic_LDFLAGS = $(tests_LDFLAGS_PRIV)
generic_LDADD = $(tests_LIBADD_PRIV)
