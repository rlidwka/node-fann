var assert = require('assert')
var fann   = require('../')

describe('conn_set', function () {
  var net
  var conn

  before(function () {
    net = new fann.standard(2,5,1)
  })

  /*
   *  Get/set connections
   */

  it('changing individual connection', function () {
    conn = net.get_weight_array()
    var oldconn = conn[3][9]
    conn[3][9] = 0.9
    net.set_weight_array(conn)
    assert.equal(conn[3][9], 0.9)
  })

  it('changing specified connection', function () {
    net.set_weight(4, 9, 0.4)
    conn = net.get_weight_array()
    assert.equal(conn[4][9], 0.4)
  })
})
