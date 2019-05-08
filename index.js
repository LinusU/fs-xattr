'use strict'

const addon = require('./build/Release/xattr')

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

exports.get = function get (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return new Promise((resolve, reject) => addon.get(path, attr, (err, result) => err ? reject(err) : resolve(result)))
}

exports.set = function set (path, attr, value) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)
  value = validateArgument('value', value)

  return new Promise((resolve, reject) => addon.set(path, attr, value, (err) => err ? reject(err) : resolve()))
}

exports.list = function list (path) {
  path = validateArgument('path', path)

  return new Promise((resolve, reject) => addon.list(path, (err, result) => err ? reject(err) : resolve(result)))
}

exports.remove = function remove (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return new Promise((resolve, reject) => addon.remove(path, attr, (err) => err ? reject(err) : resolve()))
}

/* Sync methods */

exports.getSync = function getSync (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.getSync(path, attr)
}

exports.setSync = function setSync (path, attr, value) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)
  value = validateArgument('value', value)

  return addon.setSync(path, attr, value)
}

exports.listSync = function listSync (path) {
  path = validateArgument('path', path)

  return addon.listSync(path)
}

exports.removeSync = function removeSync (path, attr) {
  path = validateArgument('path', path)
  attr = validateArgument('attr', attr)

  return addon.removeSync(path, attr)
}
