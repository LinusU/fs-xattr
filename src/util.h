#ifndef LD_UTIL_H
#define LD_UTIL_H

#include <stddef.h>

#define NAPI_VERSION 1
#include <node_api.h>

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#include <sys/types.h>
#include <sys/extattr.h>
#include <sys/errno.h>
#else
#include <errno.h>
#include <sys/xattr.h>
#endif

napi_status split_string_array(napi_env env, const char *data, size_t length, napi_value* result);

#endif
