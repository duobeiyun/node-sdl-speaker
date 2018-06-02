const sdl = require('./wrapper');
const { Writable } = require('stream');

module.exports = class Channel extends Writable {
  constructor(name, streamOption) {
    super(streamOption);
    this.name = name;
  }

  clean() {
    sdl.clean(this.name);
  }

  _write(buf, encoding, next) {
    sdl.write(this.name, buf);
    next();
  }
}
