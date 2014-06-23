
#include <errno.h>
#include <string.h>

#ifdef __APPLE__
#define E_ENOATTR ENOATTR
#define S_ENOATTR "ENOATTR"
#else
#define E_ENOATTR ENODATA
#define S_ENOATTR "ENODATA"
#endif

using namespace v8;

Local<String> errorDescription(int e) {
  return String::New(strerror(e));
}

Local<String> errorDescriptionForGet(int e) {
  switch (e) {
    case E_ENOATTR: return String::New("The extended attribute does not exist.");
    case ENOTSUP: return String::New("The file system does not support extended attributes or has the feature disabled.");
    case ERANGE: return String::New("value (as indicated by size) is too small to hold the extended attribute data.");
    case EPERM: return String::New("The named attribute is not permitted for this type of object.");
    case EINVAL: return String::New("name is invalid or options has an unsupported bit set.");
    case EISDIR: return String::New("path or fd do not refer to a regular file and the attribute in question is only applicable to files.  Similar to EPERM.");
    case ENOTDIR: return String::New("A component of path's prefix is not a directory.");
    case ENAMETOOLONG: return String::New("The length of name exceeds XATTR_MAXNAMELEN UTF-8 bytes, or a component of path exceeds NAME_MAX characters, or the entire path exceeds PATH_MAX characters.");
    case EACCES: return String::New("Search permission is denied for a component of path or the attribute is not allowed to be read (e.g. an ACL prohibits reading the attributes of this file).");
    case ELOOP: return String::New("Too many symbolic links were encountered in translating the pathname.");
    case EFAULT: return String::New("path or name points to an invalid address.");
    case EIO: return String::New("An I/O error occurred while reading from or writing to the file system.");
    default: return errorDescription(e);
  }
}

Local<String> errorDescriptionForSet(int e) {
  switch (e) {
    case EEXIST: return String::New("options contains XATTR_CREATE and the named attribute already exists.");
    case E_ENOATTR: return String::New("options is set to XATTR_REPLACE and the named attribute does not exist.");
    case ENOTSUP: return String::New("The file system does not support extended attributes or has the feature disabled.");
    case EROFS: return String::New("The file system is mounted read-only.");
    case ERANGE: return String::New("The data size of the attribute is out of range (some attributes have size restrictions).");
    case EPERM: return String::New("Attributes cannot be associated with this type of object. For example, attributes are not allowed for resource forks.");
    case EINVAL: return String::New("name or options is invalid. name must be valid UTF-8 and options must make sense.");
    case ENOTDIR: return String::New("A component of path is not a directory.");
    case ENAMETOOLONG: return String::New("name exceeded XATTR_MAXNAMELEN UTF-8 bytes, or a component of path exceeded NAME_MAX characters, or the entire path exceeded PATH_MAX characters.");
    case EACCES: return String::New("Search permission is denied for a component of path or permission to set the attribute is denied.");
    case ELOOP: return String::New("Too many symbolic links were encountered resolving path.");
    case EFAULT: return String::New("path or name points to an invalid address.");
    case EIO: return String::New("An I/O error occurred while reading from or writing to the file system.");
    case E2BIG: return String::New("The data size of the extended attribute is too large.");
    case ENOSPC: return String::New("Not enough space left on the file system.");
    default: return errorDescription(e);
  }
}

Local<String> errorCode(int e) {
  switch (e) {
    /* Standard */
    case EPERM: return String::New("EPERM");
    case ENOENT: return String::New("ENOENT");
    case ESRCH: return String::New("ESRCH");
    case EINTR: return String::New("EINTR");
    case EIO: return String::New("EIO");
    case ENXIO: return String::New("ENXIO");
    case E2BIG: return String::New("E2BIG");
    case ENOEXEC: return String::New("ENOEXEC");
    case EBADF: return String::New("EBADF");
    case ECHILD: return String::New("ECHILD");
    case EAGAIN: return String::New("EAGAIN");
    case ENOMEM: return String::New("ENOMEM");
    case EACCES: return String::New("EACCES");
    case EFAULT: return String::New("EFAULT");
    case ENOTBLK: return String::New("ENOTBLK");
    case EBUSY: return String::New("EBUSY");
    case EEXIST: return String::New("EEXIST");
    case EXDEV: return String::New("EXDEV");
    case ENODEV: return String::New("ENODEV");
    case ENOTDIR: return String::New("ENOTDIR");
    case EISDIR: return String::New("EISDIR");
    case EINVAL: return String::New("EINVAL");
    case ENFILE: return String::New("ENFILE");
    case EMFILE: return String::New("EMFILE");
    case ENOTTY: return String::New("ENOTTY");
    case ETXTBSY: return String::New("ETXTBSY");
    case EFBIG: return String::New("EFBIG");
    case ENOSPC: return String::New("ENOSPC");
    case ESPIPE: return String::New("ESPIPE");
    case EROFS: return String::New("EROFS");
    case EMLINK: return String::New("EMLINK");
    case EPIPE: return String::New("EPIPE");
    case EDOM: return String::New("EDOM");
    case ERANGE: return String::New("ERANGE");
    /* Special */
    case E_ENOATTR: return String::New(S_ENOATTR);
    case ENOTSUP: return String::New("ENOTSUP");
    case ENAMETOOLONG: return String::New("ENAMETOOLONG");
    case ELOOP: return String::New("ELOOP");
    /* Unknown */
    default: return String::New("");
  }
}

void ThrowExceptionErrno(int e) {

  Local<Object> err = v8::Exception::Error(errorDescriptionForGet(e))->ToObject();
  err->Set(String::NewSymbol("errno"), Integer::New(e));
  err->Set(String::NewSymbol("code"), errorCode(e));

  ThrowException(err);
}
