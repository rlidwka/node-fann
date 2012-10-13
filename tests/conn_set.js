/*
 *  Get/set connections
 */

var assert = require('assert');
var fann = require('../build/Release/fann');
var net = new fann.standard(2,5,1);

var conn;

// changing individual connection
conn = net.get_weight_array();
var oldconn = conn[3][9];
conn[3][9] = 0.9;
net.set_weight_array(conn);

// changing specified connection
net.set_weight(4, 9, 0.4);
conn = net.get_weight_array();
console.log(conn[3][9]);
console.log(conn[4][9]);
//assert.equal(conn[3][9], 0.9, 'set individual conn');

