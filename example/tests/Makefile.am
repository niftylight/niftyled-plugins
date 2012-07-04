# libniftyled Makefile.am - v0.3 - Daniel Hiepler <daniel@niftylight.de> 


tests_CFLAGS_PRIV = -Wall -g -O0 $(niftyled_CFLAGS) $(DEBUG_FLAGS)
tests_LDFLAGS_PRIV = -Wall
tests_LDADD_PRIV = ../src/$(PACKAGE).la $(niftyled_LIBS)


check_PROGRAMS = generic
TESTS = $(check_PROGRAMS)

generic_SOURCES = generic.c
generic_CFLAGS = $(tests_CFLAGS_PRIV)
generic_LDFLAGS = $(tests_LDFLAGS_PRIV)
generic_LDADD = $(tests_LDADD_PRIV)
