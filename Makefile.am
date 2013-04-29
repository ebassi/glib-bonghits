# preamble
ACLOCAL_AMFLAGS = -I build/autotools

NULL =

BUILT_SOURCES =

CLEANFILES =
DISTCLEANFILES =
EXTRA_DIST =

lib_LTLIBRARIES =
bin_PROGRAMS =
noinst_PROGRAMS =

TEST_PROGS =

# extra build stuff that we should put in the tarballs
EXTRA_DIST += \
	autogen.sh \
	build/autotools/as-compiler-flag.m4 \
	build/autotools/Makefile-gtest.am \
	$(NULL)

AM_CPPFLAGS = \
	-DPREFIX=\""$(prefix)"\"		\
	-DLIBDIR=\""$(libdir)"\"		\
	-DGLIB_BONGHITS_COMPILATION=1		\
	-DG_LOG_DOMAIN=\"GLib-Bonghits\"	\
	-I$(top_srcdir)				\
	-I$(top_builddir)			\
	$(GB_DEBUG_CFLAGS)			\
	$(NULL)

AM_CFLAGS = $(GB_CFLAGS) $(MAINTAINER_CFLAGS) $(GB_GCOV_CFLAGS)

# main library
source_public_h = \
	glib-bonghits/gb-ref-ptr.h \
	$(NULL)

source_private_h = \
	glib-bonghits/valgrind.h \
	$(NULL)

source_c = \
	glib-bonghits/gb-ref-ptr.c \
	$(NULL)

lib_LTLIBRARIES += libglib-bonghits-0.la
libglib_bonghits_0_la_LIBADD = $(GB_LIBS)
libglib_bonghits_0_la_SOURCES = $(source_public_h) $(source_c) $(source_private_h) $(BUILT_SOURCES)
libglib_bonghits_0_la_LDFLAGS = $(GB_LT_LDFLAGS) $(GB_GCOV_LDADD) -no-undefined

gbincludedir = $(includedir)/glib-bonghits
gbinclude_HEADERS = $(source_public_h) $(top_srcdir)/glib-bonghits/glib-bonghits.h

# tests
noinst_PROGRAMS += $(TEST_PROGS)
tests_ldadd = libglib-bonghits-0.la $(GB_LIBS)

ref_ptr_SOURCES = glib-bonghits/tests/ref-ptr.c
ref_ptr_LDADD = $(tests_ldadd)
TEST_PROGS += ref-ptr

-include $(top_srcdir)/build/autotools/Makefile-gtest.am
