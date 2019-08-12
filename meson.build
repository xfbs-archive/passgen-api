project('passgen-api', 'c')

cc = meson.get_compiler('c')

libmongoose = cc.find_library('mongoose')
libjansson = dependency('jansson')
libpassgen = dependency('passgen')

deps = [libmongoose, libjansson, libpassgen]

executable('api_server',
  'src/api_server.c',
  dependencies: deps)
