/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_mount.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(mount)
{
#define REGISTER_MNT_ENUM(e) REGISTER_NS_LONG_CONSTANT("ELEPHOS", #e, e, CONST_CS | CONST_PERSISTENT);

	REGISTER_MNT_ENUM(MS_NOUSER);
	REGISTER_MNT_ENUM(MS_ACTIVE);
	REGISTER_MNT_ENUM(MS_STRICTATIME);
	REGISTER_MNT_ENUM(MS_I_VERSION);
	REGISTER_MNT_ENUM(MS_KERNMOUNT);
	REGISTER_MNT_ENUM(MS_RELATIME);
	REGISTER_MNT_ENUM(MS_SHARED);
	REGISTER_MNT_ENUM(MS_SLAVE);
	REGISTER_MNT_ENUM(MS_PRIVATE);
	REGISTER_MNT_ENUM(MS_UNBINDABLE);
	REGISTER_MNT_ENUM(MS_POSIXACL);
	REGISTER_MNT_ENUM(MS_SILENT);
	REGISTER_MNT_ENUM(MS_REC);
	REGISTER_MNT_ENUM(MS_MOVE);
	REGISTER_MNT_ENUM(MS_BIND);
	REGISTER_MNT_ENUM(MS_NODIRATIME);
	REGISTER_MNT_ENUM(MS_NOATIME);
	REGISTER_MNT_ENUM(MS_DIRSYNC);
	REGISTER_MNT_ENUM(MS_MANDLOCK);
	REGISTER_MNT_ENUM(MS_REMOUNT);
	REGISTER_MNT_ENUM(MS_SYNCHRONOUS);
	REGISTER_MNT_ENUM(MS_NOEXEC);
	REGISTER_MNT_ENUM(MS_NODEV);
	REGISTER_MNT_ENUM(MS_NOSUID);
	REGISTER_MNT_ENUM(MS_RDONLY);

	REGISTER_MNT_ENUM(MS_RMT_MASK);

	REGISTER_MNT_ENUM(MS_MGC_VAL);
	REGISTER_MNT_ENUM(MS_MGC_MSK);

	REGISTER_MNT_ENUM(MNT_FORCE);
	REGISTER_MNT_ENUM(MNT_DETACH);
	REGISTER_MNT_ENUM(MNT_EXPIRE);
	REGISTER_MNT_ENUM(UMOUNT_NOFOLLOW);

#undef REGISTER_MNT_ENUM

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(mount)
{
#if defined(COMPILE_DL_MOUNT) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mount)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mount support", "enabled");
	php_info_print_table_end();
}
/* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_elephos_mount_arginfo, 0, 3, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, device,   IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, point,    IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type,     IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flags,    IS_LONG,   0)
	ZEND_ARG_TYPE_INFO(0, options,  IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(elephos_mount)
{
	zend_string *device = NULL, *point = NULL, *type = NULL, *options = NULL;
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SSS|lS", &device, &point, &type, &flags, &options) != SUCCESS) {
		return;
	}

	RETURN_BOOL(mount(ZSTR_VAL(device), ZSTR_VAL(point), ZSTR_VAL(type), flags, options ? ZSTR_VAL(options) : NULL) == SUCCESS);
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_elephos_umount_arginfo, 0, 1, _IS_BOOL, NULL, 0)
	ZEND_ARG_TYPE_INFO(0, device, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flags,  IS_LONG,   0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(elephos_umount)
{
	zend_string *device = NULL;
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S|l", &device, &flags) != SUCCESS) {
		return;
	}

	if (!flags) {
		RETURN_BOOL(umount(ZSTR_VAL(device)) == SUCCESS);
	} else {
		RETURN_BOOL(umount2(ZSTR_VAL(device), flags) == SUCCESS);
	}
}

/* {{{ mount_functions[]
 *
 */
const zend_function_entry mount_functions[] = {
	ZEND_NS_FENTRY("elephos", mount,      ZEND_FN(elephos_mount),      php_elephos_mount_arginfo, 0)
	ZEND_NS_FENTRY("elephos", umount,     ZEND_FN(elephos_umount),     php_elephos_umount_arginfo, 0)
	PHP_FE_END
};
/* }}} */

/* {{{ mount_module_entry
 */
zend_module_entry mount_module_entry = {
	STANDARD_MODULE_HEADER,
	"mount",
	mount_functions,
	PHP_MINIT(mount),
	NULL,
	PHP_RINIT(mount),
	NULL,
	PHP_MINFO(mount),
	PHP_MOUNT_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MOUNT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(mount)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
