var assert = require('assert');
var fann = require('../build/Release/fann');
var net = new fann.standard(1,10,1);

// default activation function
assert.equal(net.activation_function(1, 0), 'sigmoid_stepwise');

// set custom activation function and return it
assert.equal(net.activation_function(1, 0, 'sin'), 'sin');
assert.equal(net.activation_function(1, 1, 'cos'), 'cos');

// check if these activation functions are stored
assert.equal(net.activation_function(1, 0), 'sin');
assert.equal(net.activation_function(1, 1), 'cos');

// activation functions for hidden and output layers
assert.equal(net.activation_function_hidden('sigmoid'), 'sigmoid');
assert.equal(net.activation_function_output('elliot'), 'elliot');
assert.equal(net.activation_function(1, 0), 'sigmoid');
assert.equal(net.activation_function(2, 0), 'elliot');
