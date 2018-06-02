node-sdl-speaker 
======
> Output PCM stream data to speaker with SDL2. Especially for realtime audio stream. 

[![NPM](https://nodei.co/npm/sdl-speaker.png)](https://www.npmjs.com/package/sdl-speaker)

## Features

- Stream Support. You can pipe a pcm stream to audio channel.
- Multi Channels Support. You can open multi channels, it will mix all channels via `SDL_MixAudioFormat`.
- Low Latency.

## Installing / Getting started

```shell
$ npm install sdl-speaker
```

## Example

```javascript
const Speaker = require('sdl-speaker');
// Init SDL Speaker.
Speaker.init();

// Register a channel for write buf.
const testChannel = Speaker.register('test');

// pipe a pcm data to stream;
fs.createReadStream('test.pcm').pipe(testChannel);

// When you are ready..
// Start play audio
Speaker.start();
```

## Api Reference

## Speaker.init([ option ])

Init SDL. `option` is optional.

- `channels`: Channel count. default: `2`.
- `samplesRate`: Sample rate for every channel. default: `44100`.
- `samplesPerFrame`: Samples per frame. default: `1024`.

### Speaker.start()

Start play audio. Read audio data from a ring buffer. If data in ring buffer less than `samplesPerFrame`, fill 0.

### Speaker.stop()

Close and Destroy SDL player.

### Speaker.pause()

Pause SDL player.

### Speaker.resume()

Resume SDL player.

### Speaker.detach(name)

Detach audio source from SDL.

### Speaker.cleanAll()

Clean all audio sources's buffer.

#### Speaker.register(name) -> audioChannel

Register a audio source for write buffer. The `name` must unique.

#### audioChannel.write(buffer)

Write audio buffer to ring buffer. You can write before speaker start.

#### audioChannel.clean()

Clean this audio source's buffer.

## TODO

- [ ] More Test.
- [ ] Fix Travis-CI build failed.
- [x] Pipe stream to `audioSource`.
- [ ] Customise `rbuf` capability.
- [x] Self contained `SDL2`.
