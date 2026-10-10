unity_panel_deps = [
  dep_gtk,
  dep_adw,
  dep_gnomedt,
  dep_desktop_schemas,
  dep_astalbattery,
  dep_astalbluetooth,
  dep_astalnetwork,
  dep_astalpowerprofiles,
  dep_astalbrightness,
  dep_astalwp,
  dep_unity_platform_components,
]

unity_panel_sources         = files('unity-panel.c')
unity_panel_headers         = files('unity-panel.h')
unity_panel_private_sources = files('unity-panel-sheet.c', 'unity-panel-sheet-page.c')
unity_panel_generated       = []

unity_panel_includes = include_directories(
  '.',
  'pages/calendar',
  'pages/quick-settings',
  'pages/quick-settings/indicators/battery',
  'pages/quick-settings/indicators/bluetooth',
  'pages/quick-settings/indicators/brightness',
  'pages/quick-settings/indicators/dark-style',
  'pages/quick-settings/indicators/microphone',
  'pages/quick-settings/indicators/network',
  'pages/quick-settings/indicators/power-profiles',
  'pages/quick-settings/indicators/speaker',
  'pages/quick-settings/indicators/translucency',
)

subdir('pages')

unity_panel_generated += gnome.compile_resources(
  'unity-panel-resources',
  'unity-panel.gresource.xml',
  c_name: 'unity_panel',
)

unity_panel_lib = library(
  'unity-panel',
  unity_panel_sources,
  unity_panel_private_sources,
  unity_panel_generated,
  dependencies: unity_panel_deps,
  include_directories: unity_panel_includes,
  version: meson.project_version(),
  soversion: '0',
  install: true,
)

install_headers(
  unity_panel_headers,
  subdir: 'unity-panel',
)

unity_panel_dep = declare_dependency(
  link_with: unity_panel_lib,
  include_directories: unity_panel_includes,
  dependencies: unity_panel_deps,
)

pkg = import('pkgconfig')
pkg.generate(
  unity_panel_lib,
  name: 'unity-panel',
  description: 'Unity top panel',
  subdirs: 'unity-panel',
  requires: [dep_gtk, dep_adw, dep_unity_platform_components],
)
