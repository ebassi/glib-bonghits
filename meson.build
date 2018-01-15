project(
  'glib-bonghits',
  ['c'],
  version: '0.0.1',
  default_options: [
    'buildtype=debugoptimized',
    'warn_level=1',
    'c_std=c99',
  ],
)

glib_dep = dependency('glib-2.0', version: '>= 2.52.0')
gobject_dep = dependency('gobject-2.0')
gio_dep = dependency('gio-2.0')

root_inc = include_directories('.')

subdir('glib-bonghits')
