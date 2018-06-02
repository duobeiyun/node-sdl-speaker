const Speaker = require('./lib/index');
Speaker.init();
let testChannel = Speaker.register('test');
Speaker.start();

const fs = require('fs');
fs.createReadStream('output.pcm').pipe(testChannel);
Speaker.start()

setInterval(() => {}, 1000);
