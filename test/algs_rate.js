var assert = require('assert')
var fann   = require('../')

describe('algs_rate', function () {
  var net

  /*
   *  Training algorithms
   */

  before(function () {
    net = new fann.standard(1,10,1)
  })

  it("wrong algorithm, shouldn't change", function () {
    net.training_algorithm = 'back'
    assert.equal(net.training_algorithm, 'rprop')
  })

  it('changing algorithm', function () {
    net.training_algorithm = 'incremental'
    assert.equal(net.training_algorithm, 'incremental')
  })

  it('set first alg in the list', function () {
    net.training_algorithm = 1
    assert.equal(net.training_algorithm, 'batch')
  })

  it('should drop prefix', function () {
    net.training_algorithm = 'FANN_TRAIN_INCREMENTAL'
    assert.equal(net.training_algorithm, 'incremental')
  })

  /*
   *  Learning rates
   */

  it('should default to 0.7', function () {
    assert(Math.abs(net.learning_rate-0.7) < 0.0001)
  })

  it('trying to change learning rate', function () {
    net.learning_rate = '123'
    assert.equal(net.learning_rate, 123)
  })

  it('wrong rate, change to NaN', function () {
    net.learning_rate = 'hi!'
    assert(isNaN(net.learning_rate))
  })

  /*
   *  Learning momentum
   */

  it('should default to 0', function () {
    assert.equal(net.learning_momentum, 0)
  })

  it('trying to change it learning momentum', function () {
    net.learning_momentum = '123'
    assert.equal(net.learning_momentum, 123)
  })

  it('wrong momentum, change to NaN', function () {
    net.learning_momentum = 'hi!'
    assert(isNaN(net.learning_momentum))
  })
})
