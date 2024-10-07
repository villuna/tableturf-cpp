libtableturf_include = include_directories('.')
libtableturf = library('libtableturf',
  sources: ['board.cpp', 'board.hpp', 'tableturf.hpp'],
  include_directories: [libtableturf_include])

libtableturf_dep = declare_dependency(
  include_directories: libtableturf_include,
  link_with: libtableturf)

# unit tests

tests = ['board_layouts']

foreach t : tests
  e = executable(t, 'tests' / (t + '.cpp'), dependencies: [libtableturf_dep, boost_dep])
  test(t, e)
endforeach
