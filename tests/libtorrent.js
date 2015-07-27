var EventEmitter = require("events").EventEmitter;
var lt = process.binding("libtorrent");

lt.Session.prototype.__proto__ = Object.create(EventEmitter.prototype, {
  constructor: {
    value: lt.Session.constructor
  }
});
EventEmitter.call(lt.Session);

module.exports = lt;
