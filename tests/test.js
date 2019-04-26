const amnesiaclapp = require('..')

console.log(amnesiaclapp)
try {
  let p = new amnesiaclapp.Process('sekiro.exe')
  console.log('test')
  let xp = p.pointer([0x143B67DF0, 0x48, 0x28, 0x80])
  let yp = xp + 4
  let zp = yp + 4

  let x = p.readF(xp)
  let y = p.readF(yp)
  let z = p.readF(zp)

  p.writeF(xp, x + 1)
  p.writeF(yp, y + 1)
  p.writeF(zp, z + 1)
} catch (err) {
  console.log(err)
}
