include(FetchContent)

FetchContent_Declare(cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(spdlog
  URL https://github.com/gabime/spdlog/archive/v1.5.0.tar.gz
  URL_HASH SHA256=b38e0bbef7faac2b82fed550a0c19b0d4e7f6737d5321d4fd8f216b80f8aee8a
)
FetchContent_MakeAvailable(spdlog)
add_definitions(-DSPDLOG_FMT_EXTERNAL)

FetchContent_Declare(csv2
  GIT_REPOSITORY https://github.com/p-ranav/csv2.git
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(csv2)

FetchContent_Declare(catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(catch2)

# We depend only on the data files
FetchContent_Declare(reverse_geocode
  GIT_REPOSITORY https://github.com/richardpenman/reverse_geocode
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(reverse_geocode)

add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/bvlc_googlenet.caffemodel"
  OUTPUT "${CMAKE_BINARY_DIR}/bvlc_googlenet.prototxt"
  OUTPUT "${CMAKE_BINARY_DIR}/coutries.csv"
  OUTPUT "${CMAKE_BINARY_DIR}/geocode.csv"
  OUTPUT "${CMAKE_BINARY_DIR}/words.txt"
  OUTPUT "${CMAKE_BINARY_DIR}/wordnet.is_a.txt"
  COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/download_data.cmake"
)

find_package(PYthon3 COMPONENTS Interpreter)
add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/words_full_hierarchy.txt"
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generate.py"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_target(data
  DEPENDS "${CMAKE_BINARY_DIR}/bvlc_googlenet.caffemodel"
  "${CMAKE_BINARY_DIR}/bvlc_googlenet.prototxt"
  "${CMAKE_BINARY_DIR}/geocode.csv"
  "${CMAKE_BINARY_DIR}/countries.csv"
  "${CMAKE_BINARY_DIR}/wordnet.is_a.txt"
  "${CMAKE_BINARY_DIR}/words.txt"
  "${CMAKE_BINARY_DIR}/words_full_hierarchy.txt"
)

