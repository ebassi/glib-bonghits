AC_PREREQ(2.63)

AC_INIT([glib-bonghits],
        [0.0.1],
        [],
        [glib-bonghits])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_SRCDIR(configure.ac)
AC_CONFIG_MACRO_DIR([build/autotools])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.11 foreign tar-ustar dist-xz no-dist-gzip -Wno-portability])
AM_MAINTAINER_MODE([enable])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# i18n stuff
#IT_PROG_INTLTOOL(0.40.0)

#AM_GNU_GETTEXT([external])
#AM_GNU_GETTEXT_VERSION([0.17])

#GETTEXT_PACKAGE=AC_PACKAGE_NAME
#AC_SUBST([GETTEXT_PACKAGE])
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[The name of the gettext domain])

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

PKG_PROG_PKG_CONFIG([0.22])

AS_COMPILER_FLAGS([MAINTAINER_CFLAGS],
                  ["-Wall
                    -Wcast-align
                    -Wuninitialized
                    -Wno-strict-aliasing
                    -Wshadow
                    -Werror=pointer-arith
                    -Werror=missing-declarations
                    -Werror=redundant-decls
                    -Werror=empty-body
                    -Werror=format
                    -Werror=format-security
                    -Werror=format-nonliteral
                    -Werror=init-self
                    -Werror=declaration-after-statement
                    -Werror=vla"])

# don't rerun to this point if we abort
AC_CACHE_SAVE

GLIB_GSETTINGS

# Resources
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

# Basic modules check
PKG_CHECK_MODULES(GB, [
        gobject-2.0 >= 2.30.0
])

LT_INIT([disable-static])

# Save point
AC_CACHE_SAVE

AC_CONFIG_FILES([
        Makefile
])

AC_OUTPUT

echo "
  GLib-Bonghits $VERSION

  prefix: ${prefix}

  Now type 'make' to build $PACKAGE
"
