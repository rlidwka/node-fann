var assert = require('assert')
var fann   = require('../')

describe('neu_count', function () {
  var net

  before(function () {
    net = new fann.shortcut([ 1,2,3,4 ])
  })

  it('1 input layer and 4 output', function () {
    assert.equal(net.get_num_input(), 1)
    assert.equal(net.get_num_output(), 4)
  })

  it('total 1+2+3+4 + 1 = 11 neurons', function () {
    assert.equal(net.get_total_neurons(), 11)
  })

  it("44 connections, I don't remember why", function () {
    assert.equal(net.get_total_connections(), 44)
  })

  it('check network type', function () {
    assert.equal(net.get_network_type(), 'shortcut')
  })

  it('layer arrays', function () {
    assert.deepEqual(net.layers, [1,2,3,4])
    assert.deepEqual(net.get_layer_array(), [1,2,3,4])
    assert.deepEqual(net.get_bias_array(), [1,0,0,0])
  })
})
