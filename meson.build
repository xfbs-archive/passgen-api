project('passgen-api', 'c')

cc = meson.get_compiler('c')

libmongoose = cc.find_library('mongoose')

executable('api_server',
  'src/api_server.c',
  'src/sqlite3.c',
  'src/db_plugin_sqlite.c',
  dependencies: [libmongoose])
