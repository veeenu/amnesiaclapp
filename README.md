# AmnesiacLapp: a Node.js Windows process memory editor library

**Q**: *What does it do?*

**A**: It attaches to a process, reads/writes from/to addresses, calculates pointers.

**Q**: *Why the hell node.js?*

**A**: So you can make pretty web apps and visualizations with it.

**Q**: *Yeah but, dude, WHY*

**A**: I wanted to make some speedrunning tools for *Sekiro*, help myself visualize a few unclear skips. For example, the folding screen monkey skip which requires you to jump out of bounds and then learn to navigate mid air without visual aid for a few seconds. Wouldn't it be great if you could have some sort of compass for that? Except it would take ages to do in most other languages. Choose a language, choose a UI framework, setup the project, fight the lasagna of enterprise oriented abstractions for hours, go to bed, work all day the next day until you exhaust the willpower to work on it, move on. Well, not anymore. Now you only need to whip up a `WebSocket` server or something like that, instantiate an object with literally less than 10 methods, use however many of those you need to send your data out of that server, whip up one html page, connect to said server, retrieve the data, do all manners of fun business and be happy about it.

Yeah, I made a big fuss of a personal and probably unnecessary statement about the current state of desktop application development in a project's README.md. I'm also going to add that the web isn't the right solution as shipping 50M+ resource hungry Electron applications isn't my cup of tea either, but I had to yield if I didn't want to spend all my spare time finding other viable tools instead of solving problems worth solving. That said, it's been a cool exercise anyway as I got to experiment with `N-API` and it's super fun. Very recommended. Anyway, the rant will go if the project ever comes to be something useful to anyone else besides me. For now, it is probably worth for it to stay here to testify that some random guy on the internet believes that neither enterprise lasagna abstractions nor embedding whole browsers are viable solutions for desktop application development.