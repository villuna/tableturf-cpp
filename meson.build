project('tableturf', 'cpp', 
  default_options : ['cpp_std=c++17', 'default_library=static'])

boost_dep = dependency('boost')
json = subproject('json')
json_dep = json.get_variable('nlohmann_json_dep')

subdir('libtableturf')
subdir('client')
subdir('server')
