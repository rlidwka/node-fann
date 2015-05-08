{
  'targets': [
    {
      'target_name': 'fann',
      'include_dirs': [ "<!(node -e \"require('nan')\")" ],
      'direct_dependent_settings': {
        'cflags': [
          # This next command produces no output but it it will fail
          # (and cause GYP to fail) if we don't have a recent enough
          # version of this library.
          '<!@(pkg-config --atleast-version=2.1.0 fann)',
          '<!@(pkg-config --cflags fann)',
        ],
      },
      'link_settings': {
        'ldflags': [
          '<!@(pkg-config --libs-only-L --libs-only-other fann)',
        ],
        # linking against -ldoublefann against of -lfann
        'libraries': [
          '-ldoublefann',
          '<!@(echo -lfann)',
        ],
        'conditions': [
          ['"always-true"', {'libraries!': ['-lfann']}],
        ],
      },
      'sources': [
        'src/fann-accs.cc',
        'src/fann-arrs.cc',
        'src/fann-cascade.cc',
        'src/fann-create.cc',
        'src/fann-train.cc',
        'src/fann-util.cc',
        'src/fann.cc'
      ],
      'conditions': [
        [ 'OS!="win"', {
          'include_dirs': [
            '/opt/local/include'
          ],
          'library_dirs': [
            '/opt/local/lib'
          ]
        }],
      ],
    }
  ]
}
