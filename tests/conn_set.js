/*
 *  Get/set connections
 */

var assert = require('assert');
var fann = require('../build/Release/fann');
var net = new fann.standard(2,5,1);

var conn;

// changing individual connection
conn = net.get_weights();
var oldconn = conn[3][9];
conn[3][9] = 0.9;
net.set_weights(conn);

// changing specified connection
net.set_weights(4, 9, 0.4);
conn = net.get_weights();
console.log(conn[3][9]);
console.log(conn[4][9]);
//assert.equal(conn[3][9], 0.9, 'set individual conn');

