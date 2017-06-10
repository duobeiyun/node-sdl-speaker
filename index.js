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
      (err) => { console.error(err) }
    );
    this.currentState = 'close';
    this.addon.init();
  }

  start() {
    let res = this.addon.start();
    if (res === -1) {
      throw new Error('Current State is not Stop');
    } else if (res === -2) {
      throw new Error('SDL OpenAudio Failed');
    }
    this.currentState = 'start';
  }

  close() {
    this.addon.stop();
    this.currentState = 'stop';
  }

  resume() {
    let res = this.addon.resume();
    if (res === -1) {
      throw new Error('Current State is not Pause');
    }
    this.currentState = 'playing';
  }

  pause() {
    let res = this.addon.pause();
    if (res === -1) {
      throw new Error('Current State is not Playing');
    }
    if (this.currentState !== 'stop') {
      this.currentState = 'pause';
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
