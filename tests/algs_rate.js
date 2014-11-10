var assert = require('assert');
var fann = require('../build/Release/fann');
var net = new fann.standard(1,10,1);

/*
 *  Training algorithms
 */

// wrong algorithm, shouldn't change
net.training_algorithm = "back";
assert.equal(net.training_algorithm, 'rprop');

// but this should
net.training_algorithm = "incremental";
assert.equal(net.training_algorithm, 'incremental');

// set first alg in the list
net.training_algorithm = 1;
assert.equal(net.training_algorithm, 'batch');

// should drop prefix
net.training_algorithm = "FANN_TRAIN_INCREMENTAL";
assert.equal(net.training_algorithm, 'incremental');

/*
 *  Learning rates
 */

// should default to 0.7
assert(Math.abs(net.learning_rate-0.7) < 0.0001);

// trying to change it
net.learning_rate = '123';
assert.equal(net.learning_rate, 123);

// wrong rate, change to NaN
net.learning_rate = 'hi!';
assert(isNaN(net.learning_rate));

/*
 *  Learning momentum
 */

// should default to 0
assert.equal(net.learning_momentum, 0);

// trying to change it
net.learning_momentum = '123';
assert.equal(net.learning_momentum, 123);

// wrong momentum, change to NaN
net.learning_momentum = 'hi!';
assert(isNaN(net.learning_momentum));

