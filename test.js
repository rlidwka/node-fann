// 
// This neural network calculates XOR function (classic example)
//

var fann = require('./build/Release/fann')
var net = new fann.standard(2,3,1)

var data = [
  [[0, 0], [0]],
  [[0, 1], [1]],
  [[1, 0], [1]],
  [[1, 1], [0]],
]

net.train(data, { error: 0.00001 })

;[0,1].forEach(function(a) {
  ;[0,1].forEach(function(b) {
    var c = net.run([a, b]);
    console.log("xor test (" + a + "," + b + ") -> " + c)
  })
})
