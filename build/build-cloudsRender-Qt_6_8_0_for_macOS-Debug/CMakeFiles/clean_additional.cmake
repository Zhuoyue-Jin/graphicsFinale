# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Meshmerized_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Meshmerized_autogen.dir/ParseCache.txt"
  "CMakeFiles/StaticGLEW_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/StaticGLEW_autogen.dir/ParseCache.txt"
  "Meshmerized_autogen"
  "StaticGLEW_autogen"
  )
endif()
