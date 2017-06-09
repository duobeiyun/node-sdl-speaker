const SDLSpeaker = require('./build/Release/sdl-speaker.node');

class Speaker {
  constructor(opts) {
    opts = opts || {};
    this.sampleRate = opts.sampleRate || 16000;
    this.channels = opts.channels || 1;
    this.samplesPerFrame = opts.samplesPerFrame || 320;
    this.addon = new SDLSpeaker.Speaker(
      this.sampleRate,
      this.channels,
      this.samplesPerFrame,
      (err) => {
        console.error(err)
      }
    );
    this.addon.init();
  }

  start() {
    console.log('call start');
    let res = this.addon.start();
    if (res === -1) {
      throw new Error('Current State is not Stop');
    } else if (res === -2) {
      throw new Error('SDL OpenAudio Failed');
    }
  }

  close() {
    this.addon.close();
  }

  resume() {
    let res = this.addon.resume();
    if (res === -1) {
      throw new Error('Current State is not Pause');
    }
  }

  pause() {
    let res = this.addon.pause();
    if (res === -1) {
      throw new Error('Current State is not Pause');
    }
  }

  write(buf) {
    this.addon.write(buf);
  }

  clean() {
    this.addon.clean();
  }
}

module.exports = Speaker;
