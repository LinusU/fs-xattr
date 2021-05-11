/* eslint-env mocha */

import * as xattr from '../index.js'

import assert from 'node:assert'
import crypto from 'node:crypto'
import fs from 'node:fs'
import os from 'node:os'

import temp from 'fs-temp'

const attribute0 = 'user.linusu.test'
const attribute1 = 'user.linusu.secondary'
const payload0 = crypto.randomBytes(24).toString('hex')
const payload1 = crypto.randomBytes(24).toString('hex')

describe('xattr#sync', function () {
  let path

  before(function () {
    path = temp.writeFileSync('')
  })

  it('should set an attribute', function () {
    xattr.setAttributeSync(path, attribute0, payload0)
    xattr.setAttributeSync(path, attribute1, payload1)
  })

  it('should get an attribute', function () {
    const val = xattr.getAttributeSync(path, attribute0)
    assert(Buffer.isBuffer(val))
    assert.strictEqual(val.toString(), payload0)
  })

  it('should list the attributes', function () {
    const val = xattr.listAttributesSync(path)
    assert.ok(val.includes(attribute0))
    assert.ok(val.includes(attribute1))
  })

  it('should remove the attribute', function () {
    xattr.removeAttributeSync(path, attribute0)
    xattr.removeAttributeSync(path, attribute1)
  })

  it('should give useful errors', function () {
    assert.throws(function () {
      xattr.getAttributeSync(path, attribute0)
    }, function (err) {
      assert.strictEqual(err.errno, os.platform() === 'darwin' ? 93 : 61)
      assert.strictEqual(err.code, os.platform() === 'darwin' ? 'ENOATTR' : 'ENODATA')
      return true
    })
  })

  after(function (done) {
    fs.unlink(path, done)
  })
})

describe('xattr#async', function () {
  let path

  before(function () {
    path = temp.writeFileSync('')
  })

  it('should set an attribute', async function () {
    await xattr.setAttribute(path, attribute0, payload0)
    await xattr.setAttribute(path, attribute1, payload1)
  })

  it('should get an attribute', async function () {
    const val = await xattr.getAttribute(path, attribute0)

    assert(Buffer.isBuffer(val))
    assert.strictEqual(val.toString(), payload0)
  })

  it('should list the attributes', async function () {
    const list = await xattr.listAttributes(path)

    assert.ok(list.includes(attribute0))
    assert.ok(list.includes(attribute1))
  })

  it('should remove the attribute', async function () {
    await xattr.removeAttribute(path, attribute0)
    await xattr.removeAttribute(path, attribute1)
  })

  it('should give useful errors', async function () {
    let err
    try {
      await xattr.getAttribute(path, attribute0)
    } catch (_err) {
      err = _err
    }

    assert(err)
    assert.strictEqual(err.errno, os.platform() === 'darwin' ? 93 : 61)
    assert.strictEqual(err.code, os.platform() === 'darwin' ? 'ENOATTR' : 'ENODATA')
  })

  after(function (done) {
    fs.unlink(path, done)
  })
})

describe('xattr#utf8', function () {
  let path

  before(function () {
    path = temp.template('∞ %s').writeFileSync('')
  })

  it('should set an attribute', async function () {
    await xattr.setAttribute(path, attribute0, payload0)
  })

  it('should get an attribute', async function () {
    const val = await xattr.getAttribute(path, attribute0)

    assert(Buffer.isBuffer(val))
    assert.strictEqual(val.toString(), payload0)
  })

  it('should list the attributes', async function () {
    const list = await xattr.listAttributes(path)

    assert.ok(list.includes(attribute0))
  })

  it('should remove the attribute', async function () {
    await xattr.removeAttribute(path, attribute0)
  })

  it('should give useful errors', async function () {
    let err
    try {
      await xattr.getAttribute(path, attribute0)
    } catch (_err) {
      err = _err
    }

    assert(err)
    assert.strictEqual(err.errno, os.platform() === 'darwin' ? 93 : 61)
    assert.strictEqual(err.code, os.platform() === 'darwin' ? 'ENOATTR' : 'ENODATA')
  })

  after(function (done) {
    fs.unlink(path, done)
  })
})
