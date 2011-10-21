
var fann = require('../build/Release/fann');
var net = new fann.standard(1,10,1);

net.training_algorithm = "back";
console.log(net.training_algorithm, net.training_algorithm == 'RPROP');
net.training_algorithm = "incremental";
console.log(net.training_algorithm, net.training_algorithm == 'INCREMENTAL');
net.training_algorithm = 1;
console.log(net.training_algorithm, net.training_algorithm == 'BATCH');
net.training_algorithm = "FANN_TRAIN_INCREMENTAL";
console.log(net.training_algorithm, net.training_algorithm == 'INCREMENTAL');

console.log(net.training_algorithms());

console.log(net.learning_rate);
net.learning_rate = '123';
console.log(net.learning_rate, net.learning_rate == 123);
net.learning_rate = 'hi!';
console.log(net.learning_rate, isNaN(net.learning_rate));

console.log(net);

