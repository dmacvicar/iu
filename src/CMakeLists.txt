if(TIDY)
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=${CMAKE_SOURCE_DIR}/src;
    -checks=cppcoreguidelines-*,portability-*,readability-*;)
endif()

set(libiu_SOURCES
  find.cpp
  index.cpp
  resources.cpp
  index_libexif.cpp
  gps.cpp
  detect_objects.cpp
  )

add_library(libiu STATIC ${libiu_SOURCES})
set_target_properties(libiu PROPERTIES
  CXX_STANDARD 17
)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEXIF REQUIRED libexif)
target_link_libraries(libiu ${LIBEXIF_LIBRARIES})

find_package(OpenCV REQUIRED core dnn imgcodecs)
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
#target_link_libraries(libiu opencv_core opencv_dnn opencv_imgcodecs)
target_link_libraries(libiu ${OpenCV_LIBS})

find_package(xapian REQUIRED)
target_link_libraries(libiu ${XAPIAN_LIBRARIES})

add_executable(iu
  main.cpp
)
set_target_properties(iu PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(iu libiu)

include_directories(${CLI11_SOURCE_DIR}/include)
include_directories(${FMT_SOURCE_DIR}/include)
include_directories(${spdlog_SOURCE_DIR}/include)
include_directories(${csv2_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${XAPIAN_INCLUDE_DIRS})

