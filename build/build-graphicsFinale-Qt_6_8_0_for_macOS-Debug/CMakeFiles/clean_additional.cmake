# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CLOUDS_autogen"
  "CMakeFiles/CLOUDS_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/CLOUDS_autogen.dir/ParseCache.txt"
  )
endif()
