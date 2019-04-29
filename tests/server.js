const amnesiaclapp = require('..')
const process = require('process')
const WebSocket = require('ws')

function time () {
  return parseInt(process.hrtime.bigint() / (10n ** 6n))
}

const wss = new WebSocket.Server({
  port: 8080
})

let p = new amnesiaclapp.Process('sekiro.exe')

wss.on('connection', function (ws) {
  console.log('Connection!')

  function sendFn () {
    let startTime = time()
    try {
      let xp = p.pointer([0x143B67DF0, 0x48, 0x28, 0x80])
      let yp = xp + 4
      let zp = yp + 4

      let x = p.readF(xp)
      let y = p.readF(yp)
      let z = p.readF(zp)

      let o = { x: x, y: y, z: z }
      ws.send(JSON.stringify(o))
    } catch (err) {
      console.log(err)
    }
    let endTime = time()

    if (ws.readyState == WebSocket.OPEN) {
      setTimeout(sendFn, Math.max(0, 60 - (endTime - startTime)))
    }
  }

  sendFn()

})
