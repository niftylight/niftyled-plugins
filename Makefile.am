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
TARGZFILE=$(DEBTMPDIR)/@PACKAGE_NAME@_@PACKAGE_VERSION@.orig.tar.gz

deb: dist-gzip
	@echo "Building Debian package..."; \
	rm --recursive --force "$(DEBTMPDIR)" ; \
	mkdir -p "$(DEBTMPDIR)" && \
	mv "$(top_builddir)/@PACKAGE@-@PACKAGE_VERSION@.tar.gz" "$(TARGZFILE)" && \
	tar --directory "$(DEBTMPDIR)/" --extract --gunzip --file "$(TARGZFILE)" && \
	cp -r "$(top_builddir)/dist/debian/"* "$(DEBSRCDIR)/debian" && \
	mv "$(DEBSRCDIR)/debian/rules.tmpl"                 "$(DEBSRCDIR)/debian/rules" && \
	mv "$(DEBSRCDIR)/debian/control.tmpl"               "$(DEBSRCDIR)/debian/control" && \
	cd "$(DEBSRCDIR)" && \
	dpkg-buildpackage -D
