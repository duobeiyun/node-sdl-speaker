import test from 'ava';
import Speaker from './index';
import { Buffer } from 'buffer';

test('constructor', t => {
  let tmpSpeaker = new Speaker();
  t.is(tmpSpeaker.sampleRate, 16000);
  t.is(tmpSpeaker.channels, 1);
  t.is(tmpSpeaker.samplesPerFrame, 320);

  tmpSpeaker = new Speaker({sampleRate: 44100});
  t.is(tmpSpeaker.sampleRate, 44100);
})

const speaker = new Speaker();

test('start', t => {
  speaker.start();
  t.pass();
})

test('pause', t => {
  speaker.pause();
  t.pass();
})

test('resume', t => {
  speaker.resume();
  t.pass();
})

test('close', t => {
  speaker.close();
  t.pass();
})

test('write when close', t => {
  speaker.write(new Buffer.alloc(320, 0));
  t.pass();
})

test('wrtie larger than buffer queue', t => {
  speaker.write(new Buffer.alloc(320 * 100, 0));
  t.pass();
})

test(t => {
  speaker.start();
  speaker.start();
  t.pass();
})

test('clean', t => {
  speaker.clean();
  speaker.clean();
  t.pass();
})

test(t => {
  speaker.resume();
  t.pass();
})

test(t => {
  speaker.close();
  t.pass();
})

test(t => {
  speaker.start();
  speaker.pause();
  let err;
  err = t.throws(() => {
    speaker.start();
  }, Error);
  t.is(err.message, 'Current State is not Stop');
  speaker.resume();
  speaker.close()
  err = t.throws(() => {
    speaker.resume();
  }, Error) ;
  t.is(err.message, 'Current State is not Pause');
  err = t.throws(() => {
    speaker.pause();
  }, Error);
  t.is(err.message, 'Current State is not Playing');
})
