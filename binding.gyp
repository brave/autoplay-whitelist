{
  "targets": [{
    "target_name": "autoplay-whitelist",
    "type": "static_library",
    "sources": [
      "autoplay_whitelist_parser.cc",
      "autoplay_whitelist_parser.h",
      "autoplay_whitelist_data.cc",
      "autoplay_whitelist_data.h",
    ],
    "include_dirs": [
      ".",
      './node_modules/hashset-cpp'
    ],
    "dependencies": [
      "./node_modules/hashset-cpp/binding.gyp:hashset-cpp"
    ],
    "conditions": [
      ['OS=="win"', {
        }, {
          'cflags_cc': [ '-fexceptions' ]
        }
      ]
    ],
    "xcode_settings": {
      "OTHER_CFLAGS": [ "-ObjC" ],
      "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++", "-v"],
      "MACOSX_DEPLOYMENT_TARGET": "10.9",
      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
    },
  },
  {
      "target_name": "aw_node_addon",
      "sources": [
        "autoplay_whitelist_parser.cc",
        "autoplay_whitelist_parser.h",
        "autoplay_whitelist_data.cc",
        "autoplay_whitelist_data.h",
        "./node_addon/AWParserWrap.h",
        "./node_addon/AWParserWrap.cpp",
        "./node_addon/addon.cpp",
        "./node_modules/hashset-cpp/test/hashFn.cc",
        "./node_modules/hashset-cpp/test/hashFn.h"
      ],
      "include_dirs": [
        ".",
        './node_modules/hashset-cpp'
      ],
      "conditions": [
        ['OS=="win"', {
          }, {
            'cflags_cc': [ '-fexceptions' ]
          }
        ]
      ],
      "xcode_settings": {
      "OTHER_CFLAGS": [ "-ObjC" ],
      "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++", "-v"],
      "OTHER_LDFLAGS": ["-stdlib=libc++"],
      "MACOSX_DEPLOYMENT_TARGET": "10.9",
      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      },
  }],
}
