const wrapper = require('./wrapper');

/**
 * Channel for write and clean buffer.
 */
class Channel {
  constructor(name) {
    this.name = name;
  }

  /**
   * Write a sample buffer to channel.
   * @param {Buffer} buf
   */
  write(buf) {
    wrapper.speaker.write(this.name, buf);
  }

  /**
   * Clean the channel buffer.
   */
  clean() {
    wrapper.speaker.clean(this.name);
  }
}

module.exports = Channel;
