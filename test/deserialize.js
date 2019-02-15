/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
/* global describe, it, before */

var assert = require('assert');
var CAWParser = require("../build/Release/aw_node_addon").CAWParser;

describe('Client populated from deserialized data', function() {
    before(function() {
        this.addon = new CAWParser();
        this.addon.addHost("example.com");
        this.addon.addHost("youtube.com");
        this.addon.addHost("brave.com");
        this.data = this.addon.serialize();
        this.addon2 = new CAWParser();
        this.addon2.deserialize(this.data);
    })
    it('should match known host example.com', function() {
        assert(this.addon2.matchesHost("example.com"));
    })
    it('should match known host youtube.com', function() {
        assert(this.addon2.matchesHost("brave.com"));
    })
    it('should match known host brave.com', function() {
        assert(this.addon2.matchesHost("brave.com"));
    })
    it('should not match unknown host a.com', function() {
        assert.equal(this.addon2.matchesHost("a.com"), false);
    })
});
