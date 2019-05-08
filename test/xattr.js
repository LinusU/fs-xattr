/* eslint-env mocha */

'use strict'

const xattr = require('../')

const fs = require('fs')
const temp = require('fs-temp')
const assert = require('assert')
const crypto = require('crypto')

const attribute0 = 'com.linusu.test'
const attribute1 = 'com.linusu.secondary'
const payload0 = crypto.randomBytes(24).toString('hex')
const payload1 = crypto.randomBytes(24).toString('hex')

describe('xattr#sync', function () {
  let path

  before(function () {
    path = temp.writeFileSync('')
  })

  it('should set an attribute', function () {
    xattr.setSync(path, attribute0, payload0)
    xattr.setSync(path, attribute1, payload1)
  })

  it('should get an attribute', function () {
    const val = xattr.getSync(path, attribute0)
    assert(Buffer.isBuffer(val))
    assert.strictEqual(val.toString(), payload0)
  })

  it('should list the attributes', function () {
    const val = xattr.listSync(path)
    assert.ok(~val.indexOf(attribute0))
    assert.ok(~val.indexOf(attribute1))
  })

  it('should remove the attribute', function () {
    xattr.removeSync(path, attribute0)
    xattr.removeSync(path, attribute1)
  })

  it('should give useful errors', function () {
    assert.throws(function () {
      xattr.getSync(path, attribute0)
    }, function (err) {
      assert.strictEqual(err.errno, 93)
      assert.strictEqual(err.code, 'ENOATTR')
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

  it('should set an attribute', function (done) {
    xattr.set(path, attribute0, payload0, function (err) {
      assert.ifError(err)
      xattr.set(path, attribute1, payload1, done)
    })
  })

  it('should get an attribute', function (done) {
    xattr.get(path, attribute0, function (err, val) {
      assert.ifError(err)
      assert(Buffer.isBuffer(val))
      assert.strictEqual(val.toString(), payload0)
      done()
    })
  })

  it('should list the attributes', function (done) {
    xattr.list(path, function (err, list) {
      assert.ifError(err)
      assert.ok(~list.indexOf(attribute0))
      assert.ok(~list.indexOf(attribute1))
      done()
    })
  })

  it('should remove the attribute', function (done) {
    xattr.remove(path, attribute0, function (err) {
      assert.ifError(err)
      xattr.remove(path, attribute1, done)
    })
  })

  it('should give useful errors', function (done) {
    xattr.get(path, attribute0, function (err, val) {
      assert(err)
      assert.strictEqual(err.errno, 93)
      assert.strictEqual(err.code, 'ENOATTR')
      assert.strictEqual(val, undefined)
      done()
    })
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

  it('should set an attribute', function (done) {
    xattr.set(path, attribute0, payload0, done)
  })

  it('should get an attribute', function (done) {
    xattr.get(path, attribute0, function (err, val) {
      assert.ifError(err)
      assert(Buffer.isBuffer(val))
      assert.strictEqual(val.toString(), payload0)
      done()
    })
  })

  it('should list the attributes', function (done) {
    xattr.list(path, function (err, list) {
      assert.ifError(err)
      assert.ok(~list.indexOf(attribute0))
      done()
    })
  })

  it('should remove the attribute', function (done) {
    xattr.remove(path, attribute0, done)
  })

  it('should give useful errors', function (done) {
    xattr.get(path, attribute0, function (err, val) {
      assert(err)
      assert.strictEqual(err.errno, 93)
      assert.strictEqual(err.code, 'ENOATTR')
      assert.strictEqual(val, undefined)
      done()
    })
  })

  after(function (done) {
    fs.unlink(path, done)
  })
})
