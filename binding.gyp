{
  'targets': [
    {
      'target_name': 'fann',
      'direct_dependent_settings': {
        'cflags': [
          # This next command produces no output but it it will fail
          # (and cause GYP to fail) if we don't have a recent enough
          # version of sqlite.
          '<!@(pkg-config --atleast-version=2.1.0 fann)',
          '<!@(pkg-config --cflags fann)',
        ],
      },
      'link_settings': {
        'ldflags': [
          '<!@(pkg-config --libs-only-L --libs-only-other fann)',
        ],
        'libraries': [
          '<!@(pkg-config --libs-only-l fann)',
        ],
      },
      'sources': [
        'src/fann-accs.cc',
        'src/fann-arrs.cc',
        'src/fann-create.cc',
        'src/fann-cascade.cc',
        'src/fann-train.cc',
        'src/fann-util.cc',
        'src/fann.cc'
      ],
    }
  ]
}
