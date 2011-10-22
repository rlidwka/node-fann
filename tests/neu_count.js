
var fann = require('../build/Release/fann');
var net = new fann.shortcut([1,2,3,4]);

console.log(net.get_num_input());
console.log(net.get_num_output());
console.log(net.get_total_neurons());
console.log(net.get_total_connections());
console.log(net.get_network_type());

