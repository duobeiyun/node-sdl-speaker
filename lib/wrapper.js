const SDLSpeaker = require('../build/Release/sdl-speaker.node');

let speaker = null;

module.exports = {
  init(sampleRate, channels, samplesPerFrame, onError) {
    speaker = new SDLSpeaker.Speaker(sampleRate, channels, samplesPerFrame, onError);
    speaker.init();
  },
  get speaker() {
    if (speaker === null) {
      throw new Error('Speaker must init');
    }
    return speaker;
  }
}
