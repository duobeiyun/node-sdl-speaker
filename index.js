const SDLSpeaker = require('./build/Release/sdl-speaker.node');
const Channel = require('./lib/Channel');
const wrapper = require('./lib/wrapper');
const API = module.exports = {};
const channels = {};
let currentState = 'close';

API.mix = (length, ...frames) => {
  let mixed = Buffer.alloc(length, 0);
  for (let i = 0; i < frames.length; i++) {
    let frame = frames[i];
    if (frame.length !== length) {
      throw new Error('Frame length must equal to length');
    }
    SDLSpeaker.mix(mixed, frame, frames.length);
  }
  return mixed;
}

API.init = (opts, onError) => {
  opts = opts || {};
  let sampleRate = opts.sampleRate || 16000;
  let channels = opts.channels || 1;
  let samplesPerFrame = opts.samplesPerFrame || 1024;
  let onErrorCb = onError || (() => {});
  wrapper.init(sampleRate, channels, samplesPerFrame, onErrorCb);
  currentState = 'inited';
}

API.register = (name) => {
  if (channels[name]) {
    throw new Error(`Channel [${name}] is exists`);
  }
  channels[name] = new Channel(name);
  wrapper.speaker.newChannel(name);
  return channels[name];
}

API.detach = (name) => {
  if (channels[name]) {
    delete channels[name]
  }
  wrapper.speaker.removeChannel(name);
}

API.start = () => {
  let res = wrapper.speaker.start();
  if (res === -1) {
    throw new Error('Current State is not Stop');
  } else if (res === -2) {
    throw new Error('SDL OpenAudio Failed');
  }
  currentState = 'start';
}

API.stop = () => {
  wrapper.speaker.stop();
  currentState = 'stop';
}

API.resume = () => {
  let res = wrapper.speaker.resume();
  if (res === -1) {
    throw new Error('Current State is not Pause');
  }
  currentState = 'playing';
}

API.pause = () => {
  let res = wrapper.speaker.pause();
  if (res === -1) {
    throw new Error('Current State is not Playing');
  }
  if (currentState !== 'stop') {
    currentState = 'pause';
  }
}

API.cleanAll = () => {
  wrapper.speaker.cleanAll();
}
