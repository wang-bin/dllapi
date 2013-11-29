{
  'targets': [
    {
      'target_name': 'tst_sdl',
      'type': 'executable',
      'dependencies': [
        '../../src/libdllapi.gyp:dllapi',
        'dllapi/SDL/libSDL.gyp:SDL',
      ],
      'include_dirs': [
        'dllapi',
      ],
      'sources': [
        'main.cpp',
      ],
      'conditions': [
        ['OS=="win"', {

        }, {
          'cflags': [
          ],
          'ldflags': [
            '-ldl',
          ],
        }],
      ],
    },
  ],
}
