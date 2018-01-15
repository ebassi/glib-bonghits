gb_tests = [
  'ref-ptr',
  'static-string',
]

foreach t: gb_tests
  test_exe = executable(
    t,
    sources: t + '.c',
    dependencies: libgb_dep,
  )

  test(
    t,
    test_exe,
    env: [
      'G_TEST_SRCDIR=@0@'.format(meson.current_source_dir()),
      'G_TEST_BUILDDIR=@0@'.format(meson.current_build_dir()),
    ],
  )
endforeach
