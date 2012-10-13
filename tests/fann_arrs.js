var assert = require('assert');
var fann = require('../build/Release/fann');
var net = new fann.standard(1,10,1);

// activation functions
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
//console.log(fann.get_all_activation_functions());
assert.deepEqual(fann.get_all_activation_functions(), expected);

var expected = [ 'incremental', 'batch', 'rprop', 'quickprop' ]
//console.log(fann.get_all_training_algorithms());
assert.deepEqual(fann.get_all_training_algorithms(), expected);

var expected = [ 'mse', 'bit' ]
//console.log(fann.get_all_stop_functions());
assert.deepEqual(fann.get_all_stop_functions(), expected);

var expected = [ 'linear', 'tanh' ]
//console.log(fann.get_all_error_functions());
assert.deepEqual(fann.get_all_error_functions(), expected);

var expected = [ 'layer', 'shortcut' ]
//console.log(fann.get_all_network_types());
assert.deepEqual(fann.get_all_network_types(), expected);

