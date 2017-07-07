const wrapper = require('./wrapper');

class Channel {
  constructor(name) {
    this.name = name;
  }

  write(buf) {
    console.log(this.name);
    wrapper.speaker.write(this.name, buf);
  }

  clean() {
    wrapper.speaker.clean(this.name);
  }
}

module.exports = Channel;
