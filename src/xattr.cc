
#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include <stdlib.h>
#include <sys/xattr.h>

#include "error.cc"

using namespace v8;

Handle<Value> xattr_get(const Arguments& args) {
  HandleScope scope;

  String::AsciiValue aFilename(args[0]);
  String::AsciiValue aAttribute(args[1]);

  const char *filename = *aFilename;
  const char *attribute = *aAttribute;

  ssize_t valueLen;

#ifdef __APPLE__
  valueLen = getxattr(filename, attribute, NULL, 0, 0, 0);
#else
  valueLen = getxattr(filename, attribute, NULL, 0);
#endif

  if (valueLen == -1) {
    ThrowExceptionErrno(errno);
    return scope.Close(Undefined());
  }

  node::Buffer *slowBuffer = node::Buffer::New((size_t) valueLen);

#ifdef __APPLE__
  valueLen = getxattr(filename, attribute, node::Buffer::Data(slowBuffer), (size_t) valueLen, 0, 0);
#else
  valueLen = getxattr(filename, attribute, node::Buffer::Data(slowBuffer), (size_t) valueLen);
#endif

  if (valueLen == -1) {
    ThrowExceptionErrno(errno);
    return scope.Close(Undefined());
  }

  Local<Object> globalObj = Context::GetCurrent()->Global();
  Local<Function> bufferConstructor = Local<Function>::Cast(globalObj->Get(String::NewSymbol("Buffer")));
  Handle<Value> constructorArgs[3] = { slowBuffer->handle_, Integer::New(valueLen), Integer::New(0) };
  Local<Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

  return scope.Close(actualBuffer);
}

Handle<Value> xattr_set(const Arguments& args) {
  HandleScope scope;

  String::AsciiValue aFilename(args[0]);
  String::AsciiValue aAttribute(args[1]);

  const char *filename = *aFilename;
  const char *attribute = *aAttribute;

  Local<Object> bufferObj = args[2]->ToObject();
  const char *value = node::Buffer::Data(bufferObj);
  size_t valueLen = node::Buffer::Length(bufferObj);

#ifdef __APPLE__
  int res = setxattr(filename, attribute, value, valueLen, 0, 0);
#else
  int res = setxattr(filename, attribute, value, valueLen, 0);
#endif

  if (res == -1) {
    ThrowExceptionErrno(errno);
    return scope.Close(Undefined());
  }

  return scope.Close(Undefined());
}

Handle<Value> xattr_list(const Arguments& args) {
  HandleScope scope;

  String::AsciiValue aFilename(args[0]);
  const char *filename = *aFilename;

  ssize_t valueLen;

#ifdef __APPLE__
  valueLen = listxattr(filename, NULL, 0, 0);
#else
  valueLen = listxattr(filename, NULL, 0);
#endif

  if (valueLen == -1) {
    ThrowExceptionErrno(errno);
    return scope.Close(Undefined());
  }

  char *result = (char *) malloc((size_t) valueLen);

#ifdef __APPLE__
  valueLen = listxattr(filename, result, (size_t) valueLen, 0);
#else
  valueLen = listxattr(filename, result, (size_t) valueLen);
#endif

  if (valueLen == -1) {
    ThrowExceptionErrno(errno);
    return scope.Close(Undefined());
  }

  Local<Array> arr = Array::New(0);

  int arrayPos = 0;
  ssize_t valuePos = 0;
  while (valuePos < valueLen) {
    size_t len = strlen(result);
    arr->Set(arrayPos, String::New(result, len));
    valuePos += len + 1;
    arrayPos++;
  }

  arr->Set(String::NewSymbol("length"), Integer::New(arrayPos));
  free(result);

  return scope.Close(arr);
}

Handle<Value> xattr_remove(const Arguments& args) {
  HandleScope scope;

  String::AsciiValue aFilename(args[0]);
  String::AsciiValue aAttribute(args[1]);

  const char *filename = *aFilename;
  const char *attribute = *aAttribute;

#ifdef __APPLE__
  int res = removexattr(filename, attribute, 0);
#else
  int res = removexattr(filename, attribute);
#endif

  if (res == -1) {
    ThrowExceptionErrno(errno);
    return scope.Close(Undefined());
  }

  return scope.Close(Undefined());
}

void Initialize(Handle<Object> exports) {
  exports->Set(String::NewSymbol("get"), FunctionTemplate::New(xattr_get)->GetFunction());
  exports->Set(String::NewSymbol("set"), FunctionTemplate::New(xattr_set)->GetFunction());
  exports->Set(String::NewSymbol("list"), FunctionTemplate::New(xattr_list)->GetFunction());
  exports->Set(String::NewSymbol("remove"), FunctionTemplate::New(xattr_remove)->GetFunction());
}

NODE_MODULE(xattr, Initialize)
