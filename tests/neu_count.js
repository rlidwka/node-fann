var assert = require('assert');
var fann = require('../build/Release/fann');
var net = new fann.shortcut([1,2,3,4]);

// 1 input layer and 4 output
assert.equal(net.get_num_input(), 1);
assert.equal(net.get_num_output(), 4);

// total 1+2+3+4 + 1 = 11 neurons
assert.equal(net.get_total_neurons(), 11);

// 44 connections, I don't remember why
assert.equal(net.get_total_connections(), 44);

// network type
assert.equal(net.get_network_type(), 'shortcut');

// arrays
assert.deepEqual(net.layers, [1,2,3,4]);
assert.deepEqual(net.get_bias_array(), [1,0,0,0]);

