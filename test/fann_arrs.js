var assert = require('assert')
var fann   = require('../')

describe('fann_arrs', function () {
  var net

  before(function () {
    net = new fann.standard(1,10,1)
  })

  it('get all activation functions', function () {
    var expected = [
      'linear',
      'threshold',
      'threshold_symmetric',
      'sigmoid',
      'sigmoid_stepwise',
      'sigmoid_symmetric',
      'sigmoid_symmetric_stepwise',
      'gaussian',
      'gaussian_symmetric',
      'gaussian_stepwise',
      'elliot',
      'elliot_symmetric',
      'linear_piece',
      'linear_piece_symmetric',
      'sin_symmetric',
      'cos_symmetric',
      'sin',
      'cos' ]

    assert.deepEqual(fann.get_all_activation_functions(), expected)
  })

  it('get all training algoritums', function () {
    var expected = [ 'incremental', 'batch', 'rprop', 'quickprop' ]
    assert.deepEqual(fann.get_all_training_algorithms(), expected)
  })

  it('get all stop functions', function () {
    var expected = [ 'mse', 'bit' ]
    assert.deepEqual(fann.get_all_stop_functions(), expected)
  })

  it('get all error functions', function () {
    var expected = [ 'linear', 'tanh' ]
    assert.deepEqual(fann.get_all_error_functions(), expected)
  })

  it('get all network types', function () {
    var expected = [ 'layer', 'shortcut' ]
    assert.deepEqual(fann.get_all_network_types(), expected)
  })
})
