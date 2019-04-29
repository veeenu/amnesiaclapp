# AmnesiacLapp: a Node.js Windows process memory editor library

**Q**: *What does it do?*

**A**: It attaches to a process, reads/writes from/to addresses, calculates pointers.

**Q**: *Why the hell node.js?*

**A**: So you can make pretty web apps and visualizations with it.

**Q**: *Yeah but, dude, WHY*

**A**: I wanted to make some speedrunning tools for *Sekiro*, help myself visualize a few unclear skips. For example, the folding screen monkey skip which requires you to jump out of bounds and then learn to navigate mid air without visual aid for a few seconds. Wouldn't it be great if you could have some sort of compass for that? Except it would take ages to do in most other languages. Choose a language, choose a UI framework, setup the project, fight the lasagna of enterprise oriented abstractions for hours, go to bed, work all day the next day until you exhaust the willpower to work on it, move on. Well, not anymore. Now you only need to whip up a `WebSocket` server or something like that, instantiate an object with literally less than 10 methods, use however many of those you need to send your data out of that server, whip up one html page, connect to said server, retrieve the data, do all manners of fun business and be happy about it.

Yeah, I made a big fuss of a personal and probably unnecessary statement about the current state of desktop application development in a project's README.md. I'm also going to add that the web platform isn't the right solution as shipping 50M+ resource hungry Electron applications isn't my cup of tea either, but I had to yield if I didn't want to spend all my spare time finding other viable tools instead of solving problems worth solving. That said, it's been a cool exercise anyway as I got to experiment with `N-API` and it's super fun. Very recommended. Anyway, the rant will go if the project ever comes to be something useful to anyone else besides me. For now, it is probably worth for it to stay here to testify that some random guy on the internet believes that neither enterprise lasagna abstractions nor embedding whole browser engines are viable solutions for small desktop application development.

## So, how does it work then?

For the time being, the API is very simple and an example covering all of it can fit in a few lines:

```javascript
let p = new amnesiaclapp.Process('sekiro.exe')

let xp = p.pointer([0x143B67DF0, 0x48, 0x28, 0x80])
let yp = xp + 4
let zp = yp + 4

let x = p.readF(xp)
let y = p.readF(yp)
let z = p.readF(zp)

p.writeF(xp, x + 1.) // move 1 unit along the x axis

console.log({ x: x, y: y, z: z })
```

### Handling exceptions

- The constructor throws exceptions in case a suitable process is not found
- The `read` and `write` methods (`readI32`, `readI64`, `readU32`, `readF`, `readD`, resp. with `write`) throw the same exceptions that the Win32 API [`ReadProcessMemory`](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-readprocessmemory) and [`WriteProcessMemory`](https://docs.microsoft.com/en-us/windows/desktop/api/memoryapi/nf-memoryapi-writeprocessmemory) do.

### Caveat emptor

Please consider not deploying this in production. This has not been thoroughly tested, resource cleanup hasn't been thoroughly accounted for as its original intended use case was to edit memory in videogames' processes which are short lived, with unprivileged execution and zero system-wide side effects. Use at your own risk.