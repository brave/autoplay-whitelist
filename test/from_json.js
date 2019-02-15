/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
/* global describe, it, before */

var assert = require('assert');
var CAWParser = require("../build/Release/aw_node_addon").CAWParser;

describe('Client populated from autoplay.json', function() {
    before(function() {
        this.addon = new CAWParser();
        fs = require("fs");
        data = fs.readFileSync("data/autoplay.json", "utf8");
        hosts = JSON.parse(String(data))["hosts"];
        for (var i in hosts) {
            this.addon.addHost(hosts[i]);
        };
    })
    it('should match known host example.com', function() {
        assert(this.addon.matchesHost("example.com"));
    })
    it('should match known host youtube.com', function() {
        assert(this.addon.matchesHost("youtube.com"));
    })
    it('should match known host cheddar.com', function() {
        assert(this.addon.matchesHost("cheddar.com"));
    })
    it('should not match unknown host a.com', function() {
        assert.equal(this.addon.matchesHost("a.com"), false);
    })
});
