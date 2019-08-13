project('passgen-api', 'c')

cc = meson.get_compiler('c')

libmongoose = cc.find_library('mongoose')
libjansson = dependency('jansson')
libpassgen = dependency('passgen')
liblwan = dependency('lwan')
threads = dependency('threads')

deps = [libmongoose, libjansson, libpassgen]

executable('api_server',
  'src/api_server.c',
  dependencies: deps)

executable('lwan_hello',
  'src/lwan_hello.c',
  dependencies: [liblwan, threads])
