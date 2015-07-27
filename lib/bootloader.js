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
}
