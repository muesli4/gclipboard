AC_INIT([gclipboard], [0.0.1], [muesli4@gmail.com])
AC_CONFIG_MACRO_DIR([m4])

# TODO what options to we need to pass?
AM_INIT_AUTOMAKE([1.9 foreign subdir-objects])

# make sure a suitable c++ compiler is found
AC_PROG_CXX

PKG_CHECK_MODULES(gtkmm, gtkmm-3.0)
PKG_CHECK_MODULES(gdkmm, gdkmm-3.0)
PKG_CHECK_MODULES(pangomm, pangomm-1.4)

AX_BOOST_BASE([1.35.0],,[AC_MSG_ERROR([boost was not found])])

AC_CONFIG_FILES([Makefile src/Makefile po/Makefile.in data/Makefile])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.20])

GLIB_GSETTINGS

# generate configure script to produce Makefiles
AC_OUTPUT
