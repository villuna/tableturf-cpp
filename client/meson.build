raylib = subproject('raylib')
raylib_dep = raylib.get_variable('raylib_dep')


executable('tableturf', 
  sources: ['main.cpp', 'client.cpp', 'config.cpp'],
  dependencies: [raylib_dep, libtableturf_dep, json_dep])
