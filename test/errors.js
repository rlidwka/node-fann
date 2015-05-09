var assert = require('assert')
var fann   = require('../')

describe('errors', function () {
  var net

  it('load from bad file', function () {
    assert.throws(function() {
      var net = new fann.load(String(Math.random()))
    }, /failed to create neural network/i)
  })
})
