raylib = subproject('raylib')
raylib_dep = raylib.get_variable('raylib_dep')


executable('tableturf', 
  sources: [
    'main.cpp',
    'client.cpp',
    'config.cpp',
    'game.cpp',

    'ui/button.cpp',
    'ui/textbox.cpp',

    'states/main_menu.cpp',
    'states/info.cpp',
    'states/join_lobby.cpp',
    'states/lobby.cpp',
  ],
  dependencies: [raylib_dep, libtableturf_dep, json_dep])
