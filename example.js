const Speaker = require('./index');

const defaultFormat = {
  channels: 1,
  sampleRate: 16000,
  samplesPerFrame: 320,
};

Speaker.init(defaultFormat);
let manycamChannel = Speaker.register('manycam');
let micChannel = Speaker.register('microphone');

const ffmpegCapture = require('ffmpeg-node-capture');
const Mic = ffmpegCapture.Microphone;
console.log(ffmpegCapture.listMicrophone());
const mic = Mic.attach({deviceIndex: 1});
// const manyCamMic = Mic.attach({deviceIndex: 0});

let count = 0;

mic.on('frame', (f) => {
  count ++;
  micChannel.write(f);
  if (count === 5) {
    Speaker.start();
  }
})

const fs = require('fs');
let buf = fs.readFileSync('test.pcm');
console.log('buf length', buf.length);
manycamChannel.write(buf);
//
// setInterval(() => {
//   manycamChannel.write(buf.slice(offset, offset + 370));
//   offset += 370;
// }, 20);
