// libtorrent binding tests
var lt = process.binding("libtorrent");

function assert(value, message) {
    if(!value) {
        throw new Error(message || "Assertion failed.");
    }
}

function testProperty(obj, name, testValue) {
    obj[name] = testValue;
    assert(obj[name] === testValue, "Property failed: " + name);
}

(function() {
    console.log("Testing AddTorrentParams");

    var a = new lt.AddTorrentParams();
    testProperty(a, "name", "test");
    testProperty(a, "savePath", "C:/Test");
    testProperty(a, "trackerId", "test");
    testProperty(a, "url", "http://test");
    testProperty(a, "uuid", "test");
    testProperty(a, "sourceFeedUrl", "test");
    testProperty(a, "maxUploads", 99);
    testProperty(a, "maxConnections", 99);
    testProperty(a, "uploadLimit", 99);
    testProperty(a, "downloadLimit", 99);
})();
