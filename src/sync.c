#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "error.h"
#include "util.h"
#include "sync.h"

napi_value xattr_get_sync(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  assert(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  size_t filename_length = 0;
  assert(napi_get_value_string_utf8(env, args[0], NULL, 0, &filename_length) == napi_ok);
  char *filename = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], filename, filename_length + 1, NULL) == napi_ok);

  size_t attribute_length = 0;
  assert(napi_get_value_string_utf8(env, args[1], NULL, 0, &attribute_length) == napi_ok);
  char *attribute = malloc(attribute_length + 1);
  assert(napi_get_value_string_utf8(env, args[1], attribute, attribute_length + 1, NULL) == napi_ok);

  ssize_t value_length = 0;

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    value_length = extattr_get_file(filename, EXTATTR_NAMESPACE_USER, attribute, NULL, 0);
    printf("async.c: %s : value_length: %zd \n", __func__, value_length);
#elif __APPLE__
    value_length = getxattr(filename, attribute, NULL, 0, 0, 0);
#else
    value_length = getxattr(filename, attribute, NULL, 0);
#endif

  if (value_length == -1) {
    free(filename);
    free(attribute);
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  napi_value buffer;
  void* buffer_data = NULL;
  assert(napi_create_buffer(env, (size_t) value_length, &buffer_data, &buffer) == napi_ok);

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    value_length = extattr_get_file(filename, EXTATTR_NAMESPACE_USER, attribute, buffer_data, (size_t)value_length);
    printf("async.c: %s : value_length: %zd \n", __func__, value_length);
#elif __APPLE__
    value_length = getxattr(filename, attribute, buffer_data, (size_t)value_length, 0, 0);
#else
    value_length = getxattr(filename, attribute, buffer_data, (size_t)value_length);
#endif

  free(filename);
  free(attribute);

  if (value_length == -1) {
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  return buffer;
}

napi_value xattr_set_sync(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value args[3];
  assert(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  size_t filename_length = 0;
  assert(napi_get_value_string_utf8(env, args[0], NULL, 0, &filename_length) == napi_ok);
  char *filename = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], filename, filename_length + 1, NULL) == napi_ok);

  size_t attribute_length = 0;
  assert(napi_get_value_string_utf8(env, args[1], NULL, 0, &attribute_length) == napi_ok);
  char *attribute = malloc(attribute_length + 1);
  assert(napi_get_value_string_utf8(env, args[1], attribute, attribute_length + 1, NULL) == napi_ok);

  void *value = NULL;
  size_t value_length = 0;
  assert(napi_get_buffer_info(env, args[2], &value, &value_length) == napi_ok);

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    ssize_t res = extattr_set_file(filename, EXTATTR_NAMESPACE_USER, attribute, value, value_length);
#elif __APPLE__
    ssize_t res = setxattr(filename, attribute, value, value_length, 0, 0);
#else
    ssize_t res = setxattr(filename, attribute, value, value_length, 0);
#endif

  free(filename);
  free(attribute);

  if (res == -1) {
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  return NULL;
}

napi_value xattr_list_sync(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  assert(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  size_t filename_length = 0;
  assert(napi_get_value_string_utf8(env, args[0], NULL, 0, &filename_length) == napi_ok);
  char *filename = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], filename, filename_length + 1, NULL) == napi_ok);

  ssize_t result_length = 0;

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)

    printf("async.c: %s : data->attribute: %s \n", __func__, filename);
    result_length = extattr_list_file(filename, EXTATTR_NAMESPACE_USER, NULL, 0);
    printf("async.c: %s : data->attribute: %zd \n", __func__, result_length);
#elif __APPLE__
    result_length = listxattr(filename, NULL, 0, 0);
#else
    result_length = listxattr(filename, NULL, 0);
#endif

  if (result_length == -1) {
    free(filename);
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  char *result = (char *) malloc((size_t) result_length);

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    printf("file name: %s \n", result);
    result_length = extattr_list_file(filename, EXTATTR_NAMESPACE_USER, result, (size_t)result_length);
    printf("file name: %zd \n", result_length);
#elif __APPLE__
    result_length = listxattr(filename, result, (size_t) result_length, 0);
#else
    result_length = listxattr(filename, result, (size_t) result_length);
#endif

  free(filename);

  if (result_length == -1) {
    free(result);
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  napi_value array;
  assert(split_string_array(env, result, (size_t) result_length, &array) == napi_ok);

  free(result);

  return array;
}

napi_value xattr_remove_sync(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  assert(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  size_t filename_length;
  assert(napi_get_value_string_utf8(env, args[0], NULL, 0, &filename_length) == napi_ok);
  char *filename = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], filename, filename_length + 1, NULL) == napi_ok);

  size_t attribute_length;
  assert(napi_get_value_string_utf8(env, args[1], NULL, 0, &attribute_length) == napi_ok);
  char *attribute = malloc(attribute_length + 1);
  assert(napi_get_value_string_utf8(env, args[1], attribute, attribute_length + 1, NULL) == napi_ok);

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    int res = extattr_delete_file(filename, EXTATTR_NAMESPACE_USER, attribute);
#elif __APPLE__
    int res = removexattr(filename, attribute, 0);
#else
    int res = removexattr(filename, attribute);
#endif

  free(filename);
  free(attribute);

  if (res == -1) {
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  return NULL;
}
