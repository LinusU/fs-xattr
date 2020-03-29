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
  char *path = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], path, filename_length + 1, NULL) == napi_ok);

  size_t attribute_length = 0;
  assert(napi_get_value_string_utf8(env, args[1], NULL, 0, &attribute_length) == napi_ok);
  char *attribute = malloc(attribute_length + 1);
  assert(napi_get_value_string_utf8(env, args[1], attribute, attribute_length + 1, NULL) == napi_ok);

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    ssize_t error = extattr_get_file(path, EXTATTR_NAMESPACE_USER, attribute, NULL, 0);
#elif __APPLE__
    ssize_t error = getxattr(path, attribute, NULL, 0, 0, 0);
#else
    ssize_t error = getxattr(path, attribute, NULL, 0);
#endif

  if (error == -1) {
    free(path);
    free(attribute);
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  napi_value buffer;
  void* value = NULL;
  assert(napi_create_buffer(env, (size_t)error, &value, &buffer) == napi_ok);

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    error = extattr_get_file(path, EXTATTR_NAMESPACE_USER, attribute, value, (size_t)error);
#elif __APPLE__
    error = getxattr(path, attribute, value, (size_t)error, 0, 0);
#else
    error = getxattr(path, attribute, value, (size_t)error);
#endif

    free(path);
    free(attribute);

    if (error == -1) {
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
  char *path = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], path, filename_length + 1, NULL) == napi_ok);

  size_t attribute_length = 0;
  assert(napi_get_value_string_utf8(env, args[1], NULL, 0, &attribute_length) == napi_ok);
  char *attribute = malloc(attribute_length + 1);
  assert(napi_get_value_string_utf8(env, args[1], attribute, attribute_length + 1, NULL) == napi_ok);

  void *value = NULL;
  size_t size = 0;
  assert(napi_get_buffer_info(env, args[2], &value, &size) == napi_ok);

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    ssize_t error = extattr_set_file(path, EXTATTR_NAMESPACE_USER, attribute, value, size);
#elif __APPLE__
    ssize_t error = setxattr(path, attribute, value, size, 0, 0);
#else
    ssize_t error = setxattr(path, attribute, value, size, 0);
#endif

  free(path);
  free(attribute);

  if (error == -1) {
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
  char *path = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], path, filename_length + 1, NULL) == napi_ok);

  ssize_t error = -1;

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
  error = extattr_list_file(path, EXTATTR_NAMESPACE_USER, NULL, 0);
#elif __APPLE__
  error = listxattr(path, NULL, 0, 0);
#else
  error = listxattr(path, NULL, 0);
#endif

  if (error == -1) {
    free(path);
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  char *result = (char *) malloc((size_t) error);

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    error = extattr_list_file(path, EXTATTR_NAMESPACE_USER, result, (size_t)error);
#elif __APPLE__
    error = listxattr(path, result, (size_t) error, 0);
#else
    error = listxattr(path, result, (size_t) error);
#endif

  free(path);

  if (error == -1) {
    free(result);
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  napi_value array;
  assert(split_string_array(env, result, (size_t) error, &array) == napi_ok);
  free(result);

  return array;
}

napi_value xattr_remove_sync(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  assert(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  size_t filename_length;
  assert(napi_get_value_string_utf8(env, args[0], NULL, 0, &filename_length) == napi_ok);
  char *path = malloc(filename_length + 1);
  assert(napi_get_value_string_utf8(env, args[0], path, filename_length + 1, NULL) == napi_ok);

  size_t attribute_length;
  assert(napi_get_value_string_utf8(env, args[1], NULL, 0, &attribute_length) == napi_ok);
  char *attribute = malloc(attribute_length + 1);
  assert(napi_get_value_string_utf8(env, args[1], attribute, attribute_length + 1, NULL) == napi_ok);

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
    int error = extattr_delete_file(path, EXTATTR_NAMESPACE_USER, attribute);
#elif __APPLE__
    int error = removexattr(path, attribute, 0);
#else
    int error = removexattr(path, attribute);
#endif

  free(path);
  free(attribute);

  if (error == -1) {
    assert(throw_xattr_error(env, errno) == napi_ok);
    return NULL;
  }

  return NULL;
}
