/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "./autoplay_whitelist_parser.h"
#include "hash_set.h"

AutoplayWhitelistParser::AutoplayWhitelistParser() {
  mHosts.reset(new HashSet<ST_AUTOPLAY_WHITELIST_DATA>(256 /* TODO(mpilgrim) */, false));
}

AutoplayWhitelistParser::~AutoplayWhitelistParser() {
}

void AutoplayWhitelistParser::addHost(const char *inputHost) {
  if (nullptr == inputHost)
    return;
  ST_AUTOPLAY_WHITELIST_DATA hostData;
  hostData.sHost = new char[strlen(inputHost) + 1];
  strcpy(hostData.sHost, inputHost);
  if (nullptr == hostData.sHost)
    return;
  mHosts->Add(hostData);
}

bool AutoplayWhitelistParser::matchesHost(const char *inputHost) {
  ST_AUTOPLAY_WHITELIST_DATA hostData;
  hostData.sHost = new char[strlen(inputHost) + 1];
  if (nullptr == hostData.sHost)
    return false;
  strcpy(hostData.sHost, inputHost);
  return mHosts->Exists(hostData);
}

// Returns a newly allocated buffer, caller must manually delete[] the buffer
char* AutoplayWhitelistParser::serialize(unsigned int* totalSize) {
  *totalSize = 0;
  uint32_t hostsSize = 0;
  char* hosts = mHosts->Serialize(&hostsSize);
  *totalSize = sizeof(hostsSize) + hostsSize + 1;
  unsigned int pos = 0;
  char* result = new char[*totalSize];
  if (!result) {
    delete []hosts;
    return nullptr;
  }
  memset(result, 0, *totalSize);
  char sz[32];
  uint32_t dataLenSize = 1 + snprintf(sz, sizeof(sz), "%x", hostsSize);
  memcpy(result + pos, sz, dataLenSize);
  pos += dataLenSize;
  memcpy(result + pos, hosts, hostsSize);
  delete []hosts;
  return result;
}

bool AutoplayWhitelistParser::deserialize(char *buffer, size_t) {
  return deserialize(buffer);
}

bool AutoplayWhitelistParser::deserialize(char *buffer) {
  if (!buffer)
    return false;
  uint32_t hostsSize = 0;
  unsigned int pos = 0;
  sscanf(buffer, "%x", &hostsSize);
  pos += static_cast<uint32_t>(strlen(buffer) + 1);
  if (!mHosts->Deserialize(buffer + pos, hostsSize))
    return false;
  pos += hostsSize;
  return true;
}
