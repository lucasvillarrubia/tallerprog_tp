# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/taller_client_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/taller_client_autogen.dir/ParseCache.txt"
  "CMakeFiles/taller_server_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/taller_server_autogen.dir/ParseCache.txt"
  "CMakeFiles/taller_tests_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/taller_tests_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_main_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googlemock/CMakeFiles/gmock_main_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googlemock/gmock_autogen"
  "_deps/googletest-build/googlemock/gmock_main_autogen"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_main_autogen.dir/AutogenUsed.txt"
  "_deps/googletest-build/googletest/CMakeFiles/gtest_main_autogen.dir/ParseCache.txt"
  "_deps/googletest-build/googletest/gtest_autogen"
  "_deps/googletest-build/googletest/gtest_main_autogen"
  "taller_client_autogen"
  "taller_server_autogen"
  "taller_tests_autogen"
  )
endif()
