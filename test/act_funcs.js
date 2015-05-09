var assert = require('assert')
var fann   = require('../')

describe('act_funcs', function () {
  var net

  before(function () {
    net = new fann.standard(1,10,1)
  })

  it('default activation function', function () {
    assert.equal(net.activation_function(1, 0), 'sigmoid_stepwise')
  })

  it('custom activation function', function () {
    assert.equal(net.activation_function(1, 0, 'sin'), 'sin')
    assert.equal(net.activation_function(1, 1, 'cos'), 'cos')

    // check if these activation functions are stored
    assert.equal(net.activation_function(1, 0), 'sin')
    assert.equal(net.activation_function(1, 1), 'cos')
  })

  it('activation functions for hidden and output layers', function () {
    assert.equal(net.activation_function_hidden('sigmoid'), 'sigmoid')
    assert.equal(net.activation_function_output('elliot'), 'elliot')
    assert.equal(net.activation_function(1, 0), 'sigmoid')
    assert.equal(net.activation_function(2, 0), 'elliot')
  })
})
