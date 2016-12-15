dnl $Id$
dnl config.m4 for extension mount

PHP_ARG_ENABLE(mount, whether to enable mount support,
[  --enable-mount           Enable mount support])

if test "$PHP_MOUNT" != "no"; then
  PHP_NEW_EXTENSION(mount, mount.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
