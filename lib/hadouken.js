var http = require("http");
var lt   = require("./libtorrent");

var session = new lt.Session();

http.createServer(function(req, res) {
    res.writeHead(200, { "Content-Type": "text/plain" });
    res.end("Is the session paused: " + session.isPaused);
}).listen(1337, "127.0.0.1");
