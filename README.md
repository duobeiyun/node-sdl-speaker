node-sdl-speaker
======
### Output PCM stream data to speaker with SDL2.

使用SDL2播放PCM数据。主要是用于播放PCM数据流。使用[TPCircularBuffer](https://github.com/michaeltyson/TPCircularBuffer)缓存音频数据

Installation
---

```bash
$ npm install sdl-speaker
```

Example
---

```javascript
const Speaker = require('sdl-speaker');
const speaker = new Speaker({
  sampleRate: 16000,
  channels: 1,
  samplesPerFrame: 320
});

// write some buffer;
speaker.write(buffer);

// start play audio;
speaker.open();

```

API
---

### new Speaker([ option ]) -> instance;

创建一个`Speaker`实例。`option`是可选对象，包含如下配置

- `channels`: 声道数. 默认值: `1`.
- `samplesRate`: 每个声道的采样率. 默认值: `16000`.
- `samplesPerFrame`: 每一帧的采样数. 默认值: `320`.

### speaker.write(pcmBuffer)

把音频数据写入到缓冲区中。可以在`speaker.open()`之前执行，提前缓存数据。

### speaker.open()

开始播放音频。从缓冲区中读取数据。如果缓冲区的数据小于`samplesPerFrame`则填充静音。

### speaker.close()

关闭播放器。并清空缓冲区。

### speaker.pause()

暂停播放器。不清空缓冲区。

### speaker.resume()

重启播放器。

### speaker.clean()

主动清空缓冲区。

