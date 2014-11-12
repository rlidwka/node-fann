
var fann = require('../build/Release/fann')
var assert = require('assert')
var net = new fann.standard(~~(Math.random()*10)+1,~~(Math.random()*10)+1,~~(Math.random()*10)+1)
var filename = ''+Math.random()+'.net'
net.save(filename)
var net2 = new fann.load(filename)
assert.deepEqual(net.layers, net2.layers)
require('fs').unlink(filename)

