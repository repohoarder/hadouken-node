var lt = require("./libtorrent");

setInterval(function() {}, 1000);

var fp = new lt.Fingerprint("LT", 0, 1, 2, 3);
console.log(fp.name());

var session = new lt.Session();

process.on("SIGINT", function() {
    process.exit();
});
