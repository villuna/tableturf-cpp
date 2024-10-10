yaml_cpp = subproject('yaml_cpp')
yaml_cpp_dep = yaml_cpp.get_variable('yaml_cpp_dep')

libtableturf_include = include_directories('.')
libtableturf = library('libtableturf',
  sources: ['board.cpp', 'board.hpp', 'game.hpp', 'game.cpp', 'card.hpp', 'card.cpp', 'yaml_conversions.cpp', 'yaml_conversions.hpp'],
  include_directories: [libtableturf_include],
  dependencies: [boost_dep, yaml_cpp_dep])

libtableturf_dep = declare_dependency(
  include_directories: libtableturf_include,
  link_with: libtableturf)

# unit tests

tests = ['board_layouts', 'board_fin', 'card_yaml']

foreach t : tests
  e = executable(t, 'tests' / (t + '.cpp'), dependencies: [libtableturf_dep, boost_dep, yaml_cpp_dep])
  test(t, e)
endforeach
