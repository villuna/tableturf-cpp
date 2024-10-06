cmake = import('cmake')
opt_var = cmake.subproject_options()
opt_var.add_cmake_defines({'BUILD_SHARED_LIBS': true})
raylib_proj = cmake.subproject('raylib', options: opt_var)
raylib_dep = raylib_proj.dependency('raylib')

executable('tableturf', 'main.cpp',
  dependencies: [raylib_dep, libtableturf_dep])
