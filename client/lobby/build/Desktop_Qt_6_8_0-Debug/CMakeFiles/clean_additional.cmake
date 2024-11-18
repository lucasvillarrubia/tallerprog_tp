# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/duck_game_lobby_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/duck_game_lobby_autogen.dir/ParseCache.txt"
  "duck_game_lobby_autogen"
  )
endif()
