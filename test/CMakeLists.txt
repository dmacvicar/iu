list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include_directories(${Catch2_SOURCE_DIR}/single_include/catch2)
include_directories(${CMAKE_SOURCE_DIR}/src)

include(CTest)
add_executable(test test.cpp)
set_target_properties(test PROPERTIES
  CXX_STANDARD 17
  )
target_link_libraries(test PUBLIC libiu)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)

include(Catch)

catch_discover_tests(test)
