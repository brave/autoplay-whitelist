/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef AUTOPLAY_WHITELIST_PARSER_H_
#define AUTOPLAY_WHITELIST_PARSER_H_

#include <memory>

#include "autoplay_whitelist_data.h"

#define AUTOPLAY_DAT_FILE "AutoplayWhitelist.dat"
#define AUTOPLAY_DAT_FILE_VERSION "1"

template<class T>
class HashSet;

class AutoplayWhitelistParser {
public:
  AutoplayWhitelistParser();
  ~AutoplayWhitelistParser();

  void addHost(const char *inputHost);
  bool matchesHost(const char *inputHost);

  // Serializes the parsed data into a single buffer.
  // The returned buffer should be deleted.
  char* serialize(unsigned int* totalSize);

  // Deserializes the buffer, a size is not needed since a serialized
  // buffer is self described
  bool deserialize(char *buffer);

private:
  std::unique_ptr<HashSet<ST_AUTOPLAY_WHITELIST_DATA> > mHosts;
};

#endif  // AUTOPLAY_WHITELIST_PARSER_H_
