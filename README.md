node-sdl-speaker 
======
> Output PCM stream data to speaker with SDL2. Especially for realtime audio stream. 

[![NPM](https://nodei.co/npm/sdl-speaker.png)](https://www.npmjs.com/package/sdl-speaker)

## Installing / Getting started

You need SDL2 in your system. 

- Mac: `brew install sdl2`. 
- Windows: Download SDL2 from [www.libsdl.org](https://www.libsdl.org/download-2.0.php).

```shell
$ npm install sdl-speaker
```

## Example

```javascript
const Speaker = require('sdl-speaker');
const defaultFormat = {
  channels: 1,
  sampleRate: 16000,
  samplesPerFrame: 320,
};

// Init SDL Speaker.
Speaker.init(defaultFormat);

// Register a channel for write buf.
let test = Speaker.register('test');

// write some buffer;
test.write(buf);

// When you are ready..
// Start play audio
Speaker.start();
```

## Api Reference

## Speaker.init([ option ])

Init SDL. `option` is optional.

- `channels`: Channel count. default: `1`.
- `samplesRate`: Sample rate for every channel. default: `16000`.
- `samplesPerFrame`: Samples per frame. default: `320`.

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

#### Speaker.register(name) -> audioSource

Register a audio source for write buffer. The `name` must unique.

#### audioSource.write(buffer)

Write audio buffer to ring buffer. You can write before speaker start.

#### audioSource.clean()

Clean this audio source's buffer.

## TODO

- [ ] More Test.
- [ ] Fix Travis-CI build failed.
- [ ] Pipe stream to `audioSource`.
- [ ] Customise `rbuf` capability.
- [ ] Self contained `SDL2`.
