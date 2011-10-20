
var fann = require('./build/Release/fann');
var net = new fann.standard([5,10,10,4]);
var data = [
	[[ 0.0,  0.0], [ 0.0]],
	[[ 0.0,  1.0], [ 1.0]],
	[[ 1.0,  0.0], [ 1.0]],
	[[ 1.0,  1.0], [ 0.0]],
];
data = [
	[[1, 2, 3, 4, 5], [2, 1, 4, 1]],
	[[2, 6, 2, 2, 2], [1, 1, 2, 6]],
	[[5, 2, 6, 4, 4], [2, 1, 2, 4]],
	[[6, 2, 3, 6, 6], [8, 1, 4, 1]],
];
/*data = [
	[[10, 15], [5]],
	[[5, 12], [15]],
//	[[0, 1], [1]],
//	[[1, 0], [1]],
//	[[1, 1], [0]],
];*/
console.log(net.train(data, {epochs: 1000000, scale: 1}));
//console.log(net.train(data, {epochs: 1000000}));
console.log(net.run([10, 15]));
/*
console.log(net.run([ 1,  1]));
for (var i=0; i<1000000; i++) {
	net.train_once([0.5, 0.9], [0.3]);
}
	console.log(net.run([0.9,  0.5]));
	console.log(net.run([0.5,  0.9]));
*/
