{
  'targets': [
    {
      'target_name': 'SDL',
      'type': 'static_library',
      'dependencies': [
        '../../../../src/libdllapi.gyp:dllapi'
      ],
      'include_dirs': [
        '../../../../src',
      ],
      'sources': [
        'SDL.h',
        'SDL.cpp',
      ],
      'cflags': [
        '-fPIC',
      ],
      'lflags': [
      ]
    },
  ],
}
