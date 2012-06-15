#ifdef DEBUG
DEBUG_FLAGS = -ggdb -O0
#endif

my_cflags = -I$(top_srcdir)/src -I. $(niftyled_CFLAGS) $(DEBUG_FLAGS)
my_ldflags = $(niftyled_LIBS)


bin_PROGRAMS = generic

# test-target
TESTS = $(bin_PROGRAMS)
TESTS_ENVIRONMENT = "$(top_srcdir)/tests/tests.env"
EXTRA_DIST = tests.env


generic_SOURCES = generic.c
generic_CFLAGS = $(my_cflags)
generic_LDADD = $(my_ldflags)
