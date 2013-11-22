# node-fann

node-fann is a [FANN](http://leenissen.dk/fann/) bindings for [Node.js](http://nodejs.org).

FANN (Fast Artificial Neural Network Library) is a free open source neural network library, which implements multilayer artificial neural networks with support for both fully connected and sparsely connected networks.

## Installation

1. Make sure you have `glib2` and `pkg-config` installed.

   These are quite popular tools and should be available in your software repository/ports.

2. You will need [FANN library](http://leenissen.dk/fann/wp/download/) version _>= 2.1.0_ (libfann2).

3. Run `npm install fann` to install this package.

## Example

```javascript
var fann = require('fann');
var net = new fann.standard(2,3,1);

var data = [
    [[0, 0], [0]],
    [[0, 1], [1]],
    [[1, 0], [1]],
    [[1, 1], [0]],
];

net.train(data, {error: 0.00001});

console.log("xor test (0,0) -> ", net.run([0, 0]));
console.log("xor test (1,0) -> ", net.run([1, 0]));
console.log("xor test (0,1) -> ", net.run([0, 1]));
console.log("xor test (1,1) -> ", net.run([1, 1]));
```
