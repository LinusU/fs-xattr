# fs-xattr

Node.js module for manipulating extended attributes.

There are already some libraries for this, why use `fs-xattr`?

- Very useful errors
- No limits on value size
- Clean and easy api
- Proper asynchronous implementation

## Installation

```sh
npm install --save fs-xattr
```

## Usage

```javascript
const xattr = require('fs-xattr')

await xattr.set('index.js', 'com.linusu.test', 'Hello, World!')

console.log(await xattr.get('index.js', 'com.linusu.test'))
//=> Hello, World!
```

## API

### xattr.get(path, attr)

Get extended attribute `attr` from file at `path`.

Returns a `Promise` that will resolve with the value of the attribute.

### xattr.getSync(path, attr)

Synchronous version of `xattr.get`

### xattr.set(path, attr, value)

Set extended attribute `attr` to `value` on file at `path`.

`value` can be either a string or a `Buffer`.

Returns a `Promise` that will resolve when the value has been set.

### xattr.setSync(path, attr, value)

Synchronous version of `xattr.set`

### xattr.remove(path, attr)

Remove extended attribute `attr` on file at `path`.

Returns a `Promise` that will resolve when the value has been removed.

### xattr.removeSync(path, attr)

Synchronous version of `xattr.remove`

### xattr.list(path)

List all attributes on file at `path`.

Returns a `Promise` that will resolve with an array of strings, e.g. `['com.linusu.test', 'com.apple.FinderInfo']`.

### xattr.listSync(path)

Synchronous version of `xattr.list`
