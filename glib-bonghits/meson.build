config_h = configuration_data()
config_h.set_quoted('PACKAGE_NAME', meson.project_name())
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
configure_file(
  output: 'config.h',
  configuration: config_h,
)

public_headers = files([
  'gb-ref-ptr.h',
  'gb-static-string.h',
])

sources = files([
  'gb-ref-ptr.c',
  'gb-static-string.c',
])

libgb = library(
  'glib-bonghits',
  sources: sources,
  soversion: 0,
  version: meson.project_version(),
  dependencies: [glib_dep, gobject_dep, gio_dep],
  include_directories: include_directories('.'),
  c_args: [
    '-DPREFIX="@0@"'.format(get_option('prefix')),
    '-DLIBDIR="@0@"'.format(get_option('libdir')),
    '-DGLIB_BONGHITS_COMPILATION=1',
    '-DG_LOG_DOMAIN="GLib-Bonghits"',
    '-DG_LOG_USE_STRUCTURED=1',
  ],
  install: true,
)

libgb_dep = declare_dependency(
  link_with: libgb,
  include_directories: root_inc,
  dependencies: [glib_dep, gobject_dep, gio_dep],
)

install_headers(
  public_headers + ['glib-bonghits.h'],
  subdir: 'glib-bonghits',
)

subdir('tests')
