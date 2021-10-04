const path = require('path')
const addon = require(path.join(__dirname, './build/Release/xattr.node'))

function validateArgument (key, val) {
  switch (key) {
    case 'path':
      if (typeof val === 'string') return val
      throw new TypeError('`path` must be a string')
    case 'attr':
      if (typeof val === 'string') return val
      throw new TypeError('`attr` must be a string')
    case 'value':
      if (typeof val === 'string') return Buffer.from(val)
      if (Buffer.isBuffer(val)) return val
      throw new TypeError('`value` must be a string or buffer')
    default:
      throw new Error(`Unknown argument: ${key}`)
  }
}

/* Async methods */

function getAttribute (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.get(path, attr)
}

function setAttribute (path, attr, value) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)
  value = validateArgument('value', value)

  return addon.set(path, attr, value)
}

function listAttributes (path) {
  path = validateArgument('path', path)

  return addon.list(path)
}

function removeAttribute (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.remove(path, attr)
}

/* Sync methods */

function getAttributeSync (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.getSync(path, attr)
}

function setAttributeSync (path, attr, value) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)
  value = validateArgument('value', value)

  return addon.setSync(path, attr, value)
}

function listAttributesSync (path) {
  path = validateArgument('path', path)

  return addon.listSync(path)
}

function removeAttributeSync (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.removeSync(path, attr)
}

module.exports = {
  getAttribute,
  getAttributeSync,
  setAttribute,
  setAttributeSync,
  listAttributes,
  listAttributesSync,
  removeAttribute,
  removeAttributeSync
}
