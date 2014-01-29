#############
# libniftyled plugins Makefile.am
# v0.4 - Daniel Hiepler <daniel@niftylight.de>


include $(top_srcdir)/Makefile.global.am

EXTRA_DIST = \
	tests.env

tests_CFLAGS_PRIV = \
	$(niftyled_CFLAGS) \
	$(DEBUG_CFLAGS) \
	$(COMMON_CFLAGS_N)

tests_LDFLAGS_PRIV = \
	$(COMMON_LDFLAGS_N)

tests_LIBADD_PRIV = \
	$(niftyled_LIBS) \
	$(COMMON_LIBS_N)


# test-target
check_PROGRAMS = generic send
TESTS = $(check_PROGRAMS)
AM_TESTS_ENVIRONMENT = $(srcdir)/tests.env;

generic_SOURCES = generic.c
generic_CFLAGS = $(tests_CFLAGS_PRIV)
generic_LDFLAGS = $(tests_LDFLAGS_PRIV)
generic_LDADD = $(tests_LIBADD_PRIV)

send_SOURCES = send.c
send_CFLAGS = $(tests_CFLAGS_PRIV)
send_LDFLAGS = $(tests_LDFLAGS_PRIV)
send_LDADD = $(tests_LIBADD_PRIV)
