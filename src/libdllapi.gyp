{
  'targets': [
    {
      'target_name': 'dllapi',
      'type': 'static_library',
      'dependencies': [

      ],
      'defines': [

      ],
      'include_dirs': [
        '.',
      ],
      'sources': [
        'dllapi.h',
        'dllapi_global.h',
        'dllapi_p.h',
        'dllapi.cpp',
      ],
      'conditions': [
        ['OS=="win"', {

        }, { # OS != "win",
          'cflags': [
            '-fPIC',
          ],
          'ldflags': [
            '-ldl',
          ],
          'cxxflags': [
            '-fPIC',
          ],
        }], # OS=="win"
        ['OS=="android"', {
          'ldflags!': [
          ],
        }],
      ],
    },
  ],
}
