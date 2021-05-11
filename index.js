import { createRequire } from 'node:module'

const addon = createRequire(import.meta.url)('./build/Release/xattr')

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

export function getAttribute (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.get(path, attr)
}

export function setAttribute (path, attr, value) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)
  value = validateArgument('value', value)

  return addon.set(path, attr, value)
}

export function listAttributes (path) {
  path = validateArgument('path', path)

  return addon.list(path)
}

export function removeAttribute (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.remove(path, attr)
}

/* Sync methods */

export function getAttributeSync (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.getSync(path, attr)
}

export function setAttributeSync (path, attr, value) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)
  value = validateArgument('value', value)

  return addon.setSync(path, attr, value)
}

export function listAttributesSync (path) {
  path = validateArgument('path', path)

  return addon.listSync(path)
}

export function removeAttributeSync (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.removeSync(path, attr)
}
