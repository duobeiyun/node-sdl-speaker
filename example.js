const Speaker = require('./index');

const defaultFormat = {
  channels: 1,
  sampleRate: 16000,
  samplesPerFrame: 320,
};

Speaker.init(defaultFormat);
let testChannel = Speaker.register('test');

const fs = require('fs');
let buf = fs.readFileSync('test.pcm');
testChannel.write(buf);
