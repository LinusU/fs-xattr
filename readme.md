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

await xattr.set('index.js', 'user.linusu.test', 'Hello, World!')

console.log(await xattr.get('index.js', 'user.linusu.test'))
//=> Hello, World!
```

## API

### `get(path, attr)`

- `path` (`string`, required)
- `attr` (`string`, required)
- returns `Promise<Buffer>` - a `Promise` that will resolve with the value of the attribute.

Get extended attribute `attr` from file at `path`.

### `getSync(path, attr)`

- `path` (`string`, required)
- `attr` (`string`, required)
- returns `Buffer`

Synchronous version of `get`.

### `set(path, attr, value)`

- `path` (`string`, required)
- `attr` (`string`, required)
- `value` (`Buffer` or `string`, required)
- returns `Promise<void>` - a `Promise` that will resolve when the value has been set.

Set extended attribute `attr` to `value` on file at `path`.

### `setSync(path, attr, value)`

- `path` (`string`, required)
- `attr` (`string`, required)
- `value` (`Buffer` or `string`, required)

Synchronous version of `set`.

### `remove(path, attr)`

- `path` (`string`, required)
- `attr` (`string`, required)
- returns `Promise<void>` - a `Promise` that will resolve when the value has been removed.

Remove extended attribute `attr` on file at `path`.

### `removeSync(path, attr)`

- `path` (`string`, required)
- `attr` (`string`, required)

Synchronous version of `remove`.

### `list(path)`

- `path` (`string`, required)
- returns `Promise<Array<string>>` - a `Promise` that will resolve with an array of strings, e.g. `['user.linusu.test', 'com.apple.FinderInfo']`.

List all attributes on file at `path`.

### `listSync(path)`

- `path` (`string`, required)
- returns `Array<string>`

Synchronous version of `list`.

## Namespaces

For the large majority of Linux filesystem there are currently 4 supported namespaces (`user`, `trusted`, `security`, and `system`) you can use. Some other systems, like FreeBSD have only 2 (`user` and `system`).

Be sure to use a namespace that is appropriate for your supported platforms. You can read more about this in [the "Extended File Attributes" Wikipedia article](https://en.wikipedia.org/wiki/Extended_file_attributes#Implementations).

Using a namespace like `com.linusu.test` would work on macOS, but would give you the following error on Debian Linux:

> Error \[ENOTSUP]: The file system does not support extended attributes or has the feature disabled.
