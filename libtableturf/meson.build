libtableturf_include = include_directories('.')
libtableturf = library('libtableturf',
  sources: ['lib.cpp', 'tableturf.h'],
  include_directories: [libtableturf_include])

libtableturf_dep = declare_dependency(
  include_directories: libtableturf_include,
  link_with: libtableturf)
