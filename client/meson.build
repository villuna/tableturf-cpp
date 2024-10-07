cmake = import('cmake')
raylib_proj = cmake.subproject('raylib')
raylib_dep = raylib_proj.dependency('raylib')

executable('tableturf', 'main.cpp',
  dependencies: [raylib_dep, libtableturf_dep])
