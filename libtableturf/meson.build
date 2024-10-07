libtableturf_include = include_directories('.')
libtableturf = library('libtableturf',
  sources: ['board.cpp', 'board.hpp', 'game.hpp'],
  include_directories: [libtableturf_include],
  dependencies: [boost_dep])

libtableturf_dep = declare_dependency(
  include_directories: libtableturf_include,
  link_with: libtableturf)

# unit tests

tests = ['board_layouts', 'board_fin']

foreach t : tests
  e = executable(t, 'tests' / (t + '.cpp'), dependencies: [libtableturf_dep, boost_dep])
  test(t, e)
endforeach
