#############
# libniftyled plugins Makefile.am
# v0.4 - Daniel Hiepler <daniel@niftylight.de>


# AMFLAGS
ACLOCAL_AMFLAGS = -I m4


# subdirs to build
SUBDIRS = plugins


EXTRA_DIST = \
	Makefile.global.am \
	debian \
	version

# files to clean on "make distclean"
DISTCLEANFILES =

# files to clean on "make maintainer-clean"
MAINTAINERCLEANFILES = \
	-r $(DEBTMPDIR)/


# create .deb package
DEBTMPDIR = deb-tmp
DEBSRCDIR = $(DEBTMPDIR)/$(PACKAGE_NAME)-$(VERSION)-$(PACKAGE_MAJOR_VERSION)
deb: dist-gzip
	@echo "Building Debian package..."; \
	rm --recursive --force "$(DEBTMPDIR)" ; \
	if ! [ -d debian ] ; then \
	    @echo "The files needed for building a Debian package are not" \
	        "included by default in the distribution. To build a package, check" \
	        "out the project from source control."; \
	    exit 1; \
	fi; \
	mkdir -p "$(DEBTMPDIR)" && \
	mv "$(top_builddir)/$(PACKAGE)-$(VERSION).tar.gz" "$(DEBTMPDIR)/$(PACKAGE)_$(VERSION).orig.tar.gz" && \
	tar --directory "$(DEBTMPDIR)/" --extract --gunzip --file "$(DEBTMPDIR)/$(PACKAGE)_$(VERSION).orig.tar.gz" && \
	mv "$(DEBTMPDIR)/$(PACKAGE)-$(VERSION)" "$(DEBSRCDIR)" && \
	cp -r "$(top_builddir)/debian/"* "$(DEBSRCDIR)/debian" && \
	cp "$(top_builddir)/debian/control.tmpl" "$(DEBSRCDIR)/debian/control" && \
	cd "$(DEBSRCDIR)" && \
	dpkg-buildpackage -D -v"$(VERSION)-$(PACKAGE_MAJOR_VERSION)"
