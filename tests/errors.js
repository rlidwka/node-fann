var assert = require('assert')
var fann   = require('../build/Release/fann')

assert.throws(function() {
  var net = new fann.load(String(Math.random()))
}, /failed to create neural network/i)

