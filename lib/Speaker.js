const channels = {};
let autostart_counter = 6; // 120ms
let counter = 0;

module.exports.register = (channel) => {
  channels[channel.name] = channel;
  if (channel.autostart) {
    autostart_counter = channel.autostart;
  }
}
