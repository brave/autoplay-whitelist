/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

var CAWParser = require("../build/Release/aw_node_addon").CAWParser;
var addon = new CAWParser();

var fs = require("fs");
var data = fs.readFileSync("data/autoplay.json", "utf8");
var hosts = JSON.parse(String(data))["hosts"];
for (var i in hosts) {
    addon.addHost(hosts[i]);
};
console.log(addon.matchesHost("example.com"));
console.log(addon.matchesHost("youtube.com"));

addon.cleanup();
