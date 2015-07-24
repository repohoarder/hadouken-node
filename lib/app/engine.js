var http = require("http");
var lt   = require("../libtorrent");

// vars
var server;
var session;

function run() {
    session = new lt.Session();

    http.createServer(function(req, res) {
        res.writeHead(200, { "Content-Type": "text/plain" });
        res.end("Hadouken!");
    }).listen(1337, "127.0.0.1");
}

module.exports.run = run;
