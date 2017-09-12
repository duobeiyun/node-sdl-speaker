const SDLSpeaker = require('../build/Release/sdl-speaker.node');

let speaker = null;

module.exports = {
  /**
   * Init native speaker.
   * @param {Number} sampleRate
   * @param {Number} channels
   * @param {Number} samplesPerFrame
   * @param {Function} onError
   */
  init(sampleRate, channels, samplesPerFrame, onError) {
    speaker = new SDLSpeaker.Speaker(sampleRate, channels, samplesPerFrame, onError);
    speaker.init();
  },
  /**
   * Speaker must be the singleton.
   * @return {SDLSpeaker.Speaker}
   */
  get speaker() {
    if (speaker === null) {
      throw new Error('Speaker must init');
    }
    return speaker;
  }
}
