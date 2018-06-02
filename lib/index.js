const sdl = require('./wrapper');
const Channel = require('./Channel');

const Speaker = module.exports = {};
const channels = new Map();

/**
 * Init Speaker
 * @param {Object} opts
 * @param {Number} [opts.sampleRate = 16000]
 * @parma {Number} [opts.channels = 1]
 * @param {Number} [opts.samplesPerFrame = 1024]
 */
Speaker.init = (opts, onError) => {
  opts = opts || {};
  const sampleRate = opts.sampleRate || 44100;
  const channels = opts.channels || 2;
  const samplesPerFrame = opts.samplesPerFrame || 1024;
  console.log(sampleRate, channels, samplesPerFrame);
  return sdl.init(sampleRate, channels, samplesPerFrame, 0x8010);
}


/**
 * Register a channel for the speaker. All channels is identified by name.
 * @param {String} name
 * @return {Channel}
 */
Speaker.register = (name) => {
  if (channels.get(name)) {
    throw new Error(`Channel [${name}] is exists`);
  }
  channels.set(name, new Channel(name));
  sdl.addChannel(name);
  return channels.get(name);
}

/**
 * Detach a channel from the speaker.
 * @param {String|Channel} channel
 */
Speaker.detach = (channel) => {
  if (channel instanceof Channel) {
    sdl.removeChannel(channel.name);
  } else {
    sdl.removeChannel(channel);
  }
}

/**
 * Start Speaker.
 */
Speaker.start = () => {
  const res = sdl.start();
  if (res === -1) {
    throw new Error('Current State is not Stop');
  } else if (res === -2) {
    throw new Error('SDL OpenAudio Failed');
  }
}

/**
 * Stop Speaker.
 */
Speaker.stop = () => {
  sdl.stop();
}

Speaker.pause = () => {
  const res = sdl.pause();
  if (res === -1) {
    throw new Error('Current State is not Playing');
  }
}

Speaker.resume = () => {
  const res = sdl.resume();
  if (res === -1) {
    throw new Error('Current State is not Pause');
  }
}


/**
 * Clean up all channels buffer.
 */
Speaker.cleanAll = () => {
  sdl.cleanAll();
}

Speaker.AudioFormats = {
  AUDIO_U8: 0x0008,
  AUDIO_S8: 0x8008,
  AUDIO_U16LSB: 0x0010,
  AUDIO_S16LSB: 0x8010,
  AUDIO_U16MSB: 0x1010,
  AUDIO_S16MSB: 0x9010,
  AUDIO_U16: 0x0010,
  AUDIO_S16: 0x8010,
  AUDIO_S32LSB: 0x8020,
  AUDIO_S32MSB: 0x9020,
  AUDIO_S32: 0x8020
}
