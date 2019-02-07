/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

var CAWParser = require('../build/Release/aw_node_addon').CAWParser;
var fs = require('fs');
var addon = new CAWParser();
const serializedObject = fs.readFileSync('data/AutoplayWhitelist.dat');
console.log('serializedObject == ' + serializedObject);
console.log('size == ' + serializedObject.length);
console.log('deserialize == ' + addon.deserialize(serializedObject));
console.log(addon.matchesHost("example.com"));
console.log(addon.matchesHost("youtube.com"));

//console.log(addon.matchesTracker('facebook1.com', 'facebook.com'));
//console.log(addon.matchesTracker('facebook.com', 'facebook.com'));
//console.log(addon.matchesTracker('facebook.com', 'facebook1.com'));


//var buffer = fs.readFileSync('./data/TrackingProtection.dat');

//addon.deserialize(buffer);

// Call that to cleanup memory
addon.cleanup();
