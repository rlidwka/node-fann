var assert = require('assert')
var fann   = require('../')

describe('save_load', function () {
  var filename = ''+Math.random()+'.net'
  var net, net2

  it('should save net', function () {
    net = new fann.standard(~~(Math.random()*10)+1,~~(Math.random()*10)+1,~~(Math.random()*10)+1)
    net.save(filename)
  })

  it('should load net', function () {
    net2 = new fann.load(filename)
    assert.deepEqual(net.layers, net2.layers)
  })

  after(function () {
    require('fs').unlink(filename)
  })
})
