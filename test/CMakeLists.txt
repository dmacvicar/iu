
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
include_directories(${Catch2_SOURCE_DIR}/single_include/catch2)
include_directories(${CMAKE_SOURCE_DIR}/src)

include(CTest)
include(Catch)
add_executable(test test.cpp)
set_target_properties(test PROPERTIES
  CXX_STANDARD 17
  )
target_link_libraries(test libiu)

include(Catch)
catch_discover_tests(test)
