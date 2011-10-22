
var fann = require('../build/Release/fann');
var net = new fann.standard(1,10,1);

console.log(fann.get_all_activation_functions());
console.log(net.get_all_activation_functions());

console.log(fann.get_all_training_algorithms());
console.log(net.get_all_training_algorithms());

console.log(fann.get_all_stop_functions());
console.log(net.get_all_stop_functions());

console.log(fann.get_all_error_functions());
console.log(net.get_all_error_functions());

console.log(fann.get_all_network_types());
console.log(net.get_all_network_types());

