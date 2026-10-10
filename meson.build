project(
  'unity-panel',
  'c',
  version: '26',
  license: 'GPL-3.0-or-later',
  meson_version: '>= 1.0',
  default_options: [
    'c_std=gnu11',
    'warning_level=2',
  ],
)

dep_gtk = dependency('gtk4', version: '>= 4.22')
dep_adw = dependency('libadwaita-1', version: '>= 1.8')
dep_gnomedt = dependency('gnome-desktop-4')
dep_desktop_schemas = dependency('gsettings-desktop-schemas')
dep_astalbattery = dependency('astal-battery-0.1')
dep_astalpowerprofiles = dependency('astal-power-profiles-0.1')
dep_unity_platform_components = dependency('unity-platform-components')

cc = meson.get_compiler('c')

add_project_arguments(
  cc.get_supported_arguments(['-Wno-unused-parameter']),
  '-DG_LOG_DOMAIN="unity-panel"',
  '-DGNOME_DESKTOP_USE_UNSTABLE_API',
  language: 'c',
)

gnome = import('gnome')

subdir('data')
subdir('src')