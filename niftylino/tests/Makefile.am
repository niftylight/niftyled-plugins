#ifdef DEBUG
DEBUG_FLAGS = -ggdb -O0
#endif

my_cflags = -I$(top_srcdir)/src -I. $(niftyled_CFLAGS) $(DEBUG_FLAGS)
my_ldflags = $(niftyled_LIBS)


check_PROGRAMS =

# test-target
TESTS = $(check_PROGRAMS)
TESTS_ENVIRONMENT = "$(top_srcdir)/tests/tests.env"
EXTRA_DIST = tests.env




