# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT([imlib2], [1.4.7], [enlightenment-devel@lists.sourceforge.net])
AC_CONFIG_SRCDIR(configure.ac)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_C_BIGENDIAN
AC_PROG_CC
AM_PROG_AS

AC_HEADER_STDC
AC_C_CONST

dnl Next two lines is a hack to prevent libtool checking for CXX/F77
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
LT_INIT

VERSION_CUR=5
VERSION_REV=7
VERSION_AGE=4
lt_version=${VERSION_CUR}:${VERSION_REV}:${VERSION_AGE}
AC_SUBST(lt_version)

PKG_PROG_PKG_CONFIG


mmx=no
amd64=no
case $host_cpu in
  i*86)   mmx="yes";;
  x86_64) amd64="yes";;
  amd64) amd64="yes";;
esac

case $host_os in
  solaris*) mmx="no";;
  darwin*) mmx="no";;
esac

AC_ARG_ENABLE([mmx],
  [AS_HELP_STRING([--enable-mmx], [attempt compiling using x86 mmx assembly @<:@default=auto@:>@])],
  [
   if test x$enableval = xyes; then
     mmx=yes
     # Cannot compile with both options enabled
     amd64=no
   else
     mmx=no
   fi
  ]
)

AC_ARG_ENABLE([amd64],
  [AS_HELP_STRING([--enable-amd64], [attempt compiling using amd64 assembly @<:@default=auto@:>@])],
  [
   if test x$enableval = xyes; then
     amd64=yes
     mmx=no
   else
     amd64=no
   fi
  ]
)

AC_MSG_CHECKING(whether to enable x86 mmx support)
if test x$mmx = xyes; then
  AC_DEFINE(DO_MMX_ASM, 1, [enabling MMX Assembly])
fi
AC_MSG_RESULT($mmx)
AM_CONDITIONAL(BUILD_MMX, test x$mmx = xyes)

AC_MSG_CHECKING(whether to enable amd64 asm support)
if test x$amd64 = xyes; then
  AC_DEFINE(DO_AMD64_ASM, 1, [enabling AMD64 Assembly])
fi
AC_MSG_RESULT($amd64)
AM_CONDITIONAL(BUILD_AMD64, test x$amd64 = xyes)

# check for freetype
PKG_CHECK_MODULES(FREETYPE, freetype2)

AC_PATH_X
AC_PATH_XTRA

if test "x$have_x" = "xno"; then
   # If --without-x was passed, this will be "disabled" instead of "no" --
   # so always treat "no" as an error
   AC_MSG_ERROR(
[no X support found.  Use --x-includes and --x-libraries to specify the X
path on your system, or --without-x to disable X support.])
fi

if test "x$have_x" = "xyes"; then
  AC_DEFINE(BUILD_X11, 1, [enabling X11 support])
else
  have_x="no"
fi
AM_CONDITIONAL(BUILD_X11, test "x$have_x" = "xyes")

AC_CHECK_LIB(dl, dlopen, DLOPEN_LIBS=-ldl)
AC_SUBST(DLOPEN_LIBS)


EC_C_WARNINGS()
EC_C_VISIBILITY(yes)

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES([
Makefile
imlib2.pc
src/Makefile
src/lib/Makefile
src/bin/Makefile
src/modules/Makefile
src/modules/filters/Makefile
data/Makefile
data/fonts/Makefile
data/images/Makefile
doc/Makefile
imlib2-config
README
imlib2.spec
])
AC_CONFIG_COMMANDS([default],[
chmod +x imlib2-config
],[])
AC_OUTPUT

#####################################################################
## Info

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options Summary:"
echo
echo "Use X86 MMX for speed.....: $mmx"
echo "Use AMD64 for speed.......: $amd64"
echo
echo "Use visibility hiding.....: $enable_visibility_hiding"
echo
echo
echo "Installation Path.........: $prefix"
echo
echo "Compilation...............: make"
echo "Installation..............: make install"
echo
