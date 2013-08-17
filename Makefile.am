#############
# libniftyled plugins Makefile.am
# v0.4 - Daniel Hiepler <daniel@niftylight.de>


# AMFLAGS
ACLOCAL_AMFLAGS = -I m4


# subdirs to build
SUBDIRS = plugins


EXTRA_DIST = \
	Makefile.global.am \
	gentoo \
	debian \
	version

# files to clean on "make distclean"
DISTCLEANFILES =

# files to clean on "make maintainer-clean"
MAINTAINERCLEANFILES = \
	-r $(DEBTMPDIR)/


# indent source & header-files
INDENT_C_ARGS=-pmt -bl -bls -cli8 -cbi0 -bli0 -cs -fca -i8 -sc -npsl -nut -npcs \
		-nsaf -nsai -cd2 -nce -ncdw -lc80 -nprs -nsaw -il0 -nbbo -bap \
		-hnl -lp -blf -ip0 -nbfda
INDENT_H_ARGS=-pmt -bl -bls -cli8 -cbi0 -bli0 -cs -fca -i8 -sc -npsl -nut -npcs \
		-nsaf -nsai -cd2 -nce -ncdw -l800 -lc800 -bad -nprs -nsaw \
		-di32 -hnl -nbc -nbfda -nbfde

.PHONY: indent
indent:
	@echo Indenting source-files...
	find $(top_srcdir)/plugins -type f -and -name '*.[h]*' -not -empty -exec indent $(INDENT_H_ARGS) {} \;
	find $(top_srcdir)/plugins -type f -and -name '*.[c]*' -not -empty -exec indent $(INDENT_C_ARGS) {} \;


# create .deb package
# needs dpkg-dev, debhelper
DEBTMPDIR=$(abs_top_builddir)/deb-tmp
DEBSRCDIR=$(DEBTMPDIR)/@PACKAGE_NAME@-@PACKAGE_VERSION@
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
	mv "$(top_builddir)/@PACKAGE@-@PACKAGE_VERSION@.tar.gz" "$(DEBTMPDIR)/@PACKAGE_NAME@_@PACKAGE_VERSION@.orig.tar.gz" && \
	tar --directory "$(DEBTMPDIR)/" --extract --gunzip --file "$(DEBTMPDIR)/@PACKAGE_NAME@_@PACKAGE_VERSION@.orig.tar.gz" && \
	cp -r "$(top_builddir)/debian/"* "$(DEBSRCDIR)/debian" && \
	cp "$(top_builddir)/debian/rules.tmpl" "$(DEBSRCDIR)/debian/rules" && \
	cp "$(top_builddir)/debian/control.tmpl" "$(DEBSRCDIR)/debian/control" && \
	cd "$(DEBSRCDIR)" && \
	dpkg-buildpackage -D -v"@PACKAGE_VERSION@-@DEBIAN_REVISION@"
