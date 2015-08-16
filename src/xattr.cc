
#include <node.h>
#include <nan.h>
#include <node_buffer.h>

#include <errno.h>
#include <stdlib.h>
#include <sys/xattr.h>

#include "error.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::String;

using v8::Local;
using v8::Object;
using v8::Integer;
using v8::Value;
using v8::Function;
using v8::Array;

NAN_METHOD(xattr_get) {
  Nan::Utf8String aFilename(info[0]);
  Nan::Utf8String aAttribute(info[1]);

  const char *filename = *aFilename;
  const char *attribute = *aAttribute;

  ssize_t valueLen;

#ifdef __APPLE__
  valueLen = getxattr(filename, attribute, NULL, 0, 0, 0);
#else
  valueLen = getxattr(filename, attribute, NULL, 0);
#endif

  if (valueLen == -1) {
    return ThrowExceptionErrno(errno);
  }

  Local<Object> slowBuffer = Nan::NewBuffer((uint32_t) valueLen).ToLocalChecked();

#ifdef __APPLE__
  valueLen = getxattr(filename, attribute, node::Buffer::Data(slowBuffer), (size_t) valueLen, 0, 0);
#else
  valueLen = getxattr(filename, attribute, node::Buffer::Data(slowBuffer), (size_t) valueLen);
#endif

  if (valueLen == -1) {
    return ThrowExceptionErrno(errno);
  }

  Local<Object> globalObj = Nan::GetCurrentContext()->Global();
  Local<Function> bufferConstructor = Local<Function>::Cast(globalObj->Get(Nan::New("Buffer").ToLocalChecked()));
  Handle<Value> constructorArgs[3] = { slowBuffer, Nan::New<Integer>((int32_t) valueLen), Nan::New<Integer>(0) };
  Local<Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

  info.GetReturnValue().Set(actualBuffer);
}

NAN_METHOD(xattr_set) {
  Nan::Utf8String aFilename(info[0]);
  Nan::Utf8String aAttribute(info[1]);

  const char *filename = *aFilename;
  const char *attribute = *aAttribute;

  Local<Object> bufferObj = info[2]->ToObject();
  const char *value = node::Buffer::Data(bufferObj);
  size_t valueLen = node::Buffer::Length(bufferObj);

#ifdef __APPLE__
  int res = setxattr(filename, attribute, value, valueLen, 0, 0);
#else
  int res = setxattr(filename, attribute, value, valueLen, 0);
#endif

  if (res == -1) {
    return ThrowExceptionErrno(errno);
  }
}

NAN_METHOD(xattr_list) {
  Nan::Utf8String aFilename(info[0]);
  const char *filename = *aFilename;

  ssize_t valueLen;

#ifdef __APPLE__
  valueLen = listxattr(filename, NULL, 0, 0);
#else
  valueLen = listxattr(filename, NULL, 0);
#endif

  if (valueLen == -1) {
    return ThrowExceptionErrno(errno);
  }

  char *result = (char *) malloc((size_t) valueLen);

#ifdef __APPLE__
  valueLen = listxattr(filename, result, (size_t) valueLen, 0);
#else
  valueLen = listxattr(filename, result, (size_t) valueLen);
#endif

  if (valueLen == -1) {
    return ThrowExceptionErrno(errno);
  }

  Local<Array> arr = Nan::New<Array>(0);

  int arrayPos = 0;
  ssize_t valuePos = 0;
  while (valuePos < valueLen) {
    size_t len = strlen(result + valuePos);
    Nan::Set(arr, arrayPos, Nan::New<String>(result + valuePos, len).ToLocalChecked());
    valuePos += len + 1;
    arrayPos++;
  }

  Nan::Set(arr, Nan::New("length").ToLocalChecked(), Nan::New<Integer>(arrayPos));
  free(result);

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(xattr_remove) {
  Nan::Utf8String aFilename(info[0]);
  Nan::Utf8String aAttribute(info[1]);

  const char *filename = *aFilename;
  const char *attribute = *aAttribute;

#ifdef __APPLE__
  int res = removexattr(filename, attribute, 0);
#else
  int res = removexattr(filename, attribute);
#endif

  if (res == -1) {
    return ThrowExceptionErrno(errno);
  }
}

NAN_MODULE_INIT(Initialize) {
  Nan::Set(target, Nan::New("get").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(xattr_get)).ToLocalChecked());
  Nan::Set(target, Nan::New("set").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(xattr_set)).ToLocalChecked());
  Nan::Set(target, Nan::New("list").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(xattr_list)).ToLocalChecked());
  Nan::Set(target, Nan::New("remove").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(xattr_remove)).ToLocalChecked());
}

NODE_MODULE(xattr, Initialize)
