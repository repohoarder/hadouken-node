/*
This is the Hadouken bootloader. It is responsible for loading Hadouken and
keeping it updated. The default bootloader configuration will track the
"master" branch in the Hadouken repository, and keep your instance synced with
that. The bootloader can also load Hadouken from a directory, in which case
autoupdating is disabled.

The bootloader will start a child process that runs Hadouken. This is useful
to monitor and reload Hadouken if it crashes.
*/

"use strict";

var fork = require("child_process").fork;
var fs   = require("fs");
var http = require("http");
var path = require("path");

// CONSTANTS
const BOOTLOADER_CONFIG = "bootloader.json";

// The default configuration. This can be overriden by placing
// a bootloader.json file next to this file.
var defaultConfig = {
    type: "git",
    git: {
        repository: "https://github.com/hadouken/hadouken",
        branch: "master"
    }
};

// Check if we have an overriden bootloader configuration.
var overriddenBootloaderConfig = path.join(__dirname, BOOTLOADER_CONFIG);
fs.readFile(overriddenBootloaderConfig, function(err, data) {
    if(err) {
        return load(defaultConfig);
    }

    var config = JSON.parse(data);
    return load(config);
});

// The currently running Hadouken instance.
var hadouken;
// The source for our maintenance page (assigned further down).
var html = "";

function load(config) {
    switch(config.type) {
        case "dir":
            var entryPoint = path.join(config.dir, "hadouken.js");
            console.log("Launching Hadouken from %s.", entryPoint);
            hadouken = fork(entryPoint);
            break;

        case "git":
            break;
    }

    hadouken.on("message", function(ev) {
        console.log(ev.event);
    });

    hadouken.on("exit", function(exitCode, signal) {
        hadouken.pid = -1;
    });

    // Start maintenance server
    if(config.maintenanceServer && config.maintenanceServer.enabled) {
        loadMaintenanceServer(config.maintenanceServer);
    }
}

function loadMaintenanceServer(maintenanceConfiguration) {
    http.createServer(function(req, res) {
        if(req.method === "GET" && req.url === "/api/list") {
            res.writeHead(200, { "Content-Type": "application/json" });
            return res.end(JSON.stringify({
                pid: hadouken.pid
            }));
        }

        if(req.method === "POST" && req.url === "/api/kill") {
            hadouken.send({ action: "shutdown" });

            res.writeHead(200);
            return res.end();
        }

        if(req.url === "/favicon.ico") {
            res.writeHead(404);
            return res.end();
        }

        res.writeHead(200, { "Content-Type": "text/html" });
        res.end(html);
    }).listen(1337, "127.0.0.1");
}

/*
Maintenance page source kept at the bottom to not clutter the file.
*/
html = '<!DOCTYPE html>' +
'<html>' +
  '<head>' +
    '<title>Hadouken Maintenance</title>' +
    '<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">' +
    '<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap-theme.min.css">' +
    '<script src="https://code.jquery.com/jquery-2.1.4.min.js"></script>' +
    '<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>' +
    '<script type="text/javascript">' +
    '$(document).ready(function(event) {' +
        '$.getJSON("/api/list", function(data) {' +
            '$(".pid").text(data.pid);' +
        '});' +

        '$(".hdkn-kill").on("click", function(e) {' +
            '$.post("/api/kill", function() {' +
                '$(".pid").text("-1");' +
            '});' +
        '});' +
    '});' +
    '</script>' +
  '</head>' +
  '<body>' +
    '<div class="container">' +
      '<h1 class="page-header">Hadouken Maintenance Server</h1>' +
      '<table>' +
        '<tbody>' +
          '<tr>' +
            '<td style="width: 100px; font-weight: bold;">PID</td>' +
            '<td><span class="pid"></span></td>' +
            '<td>' +
              '<button type="button" class="btn btn-danger btn-sm hdkn-kill">Kill</button>' +
            '</td>' +
          '</tr>' +
        '</thead>' +
      '</tbody>' +
    '</div>' +
  '</body>' +
'</html>';

