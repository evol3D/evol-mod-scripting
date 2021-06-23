project('evol-mod-script', 'c',
version : '0.1',
default_options : ['warning_level=3', 'c_std=c11'])

src = subproject('evol').get_variable('evolmod_subdir') + '/meson.build'
dst = meson.current_source_dir() + '/import/'
run_command( 'python3', meson.source_root() + '/subprojects/evol/buildscripts/copy.py', src, dst)
subdir('import')

# Provided by evol
luajit_dep = dependency('luajit')

mod_src = [
  'src/mod.c',

  'src/collision_callbacks/collision_callbacks.c',
]

mod_incdir = [
  '..',
  'include',
]

mod_deps = [
  evmod_deps,

  luajit_dep,
]

module = shared_module(
  'evmodscript', mod_src,
  include_directories: mod_incdir,
  dependencies: mod_deps,
)

mod_dep = declare_dependency(
  include_directories: mod_incdir,
)

meson.override_dependency('evmod_script', mod_dep)

configure_file(input: 'src/evol_api.lua', output: 'evol_api.lua', copy:true)
