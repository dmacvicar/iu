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
  location.cpp
  detect_objects.cpp
  detect_quality.cpp
  # opencv contrib
  ${CMAKE_BINARY_DIR}/_deps/opencv_contrib-src/modules/quality/src/qualitybrisque.cpp
  )

add_library(libiu STATIC ${libiu_SOURCES})
set_target_properties(libiu PROPERTIES
  CXX_STANDARD 17
)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEXIF REQUIRED IMPORTED_TARGET libexif)
message(STATUS "Libexif library status:")
message(STATUS "    libraries: ${LIBEXIF_LIBRARIES}")
message(STATUS "    include path: ${LIBEXIF_INCLUDEDIR}")
target_link_libraries(libiu PkgConfig::LIBEXIF)

find_package(OpenCV REQUIRED core dnn imgcodecs ml)
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
#target_link_libraries(libiu opencv_core opencv_dnn opencv_imgcodecs)
target_link_libraries(libiu ${OpenCV_LIBS})

find_package(xapian REQUIRED)
message(STATUS "Xapian library status:")
message(STATUS "    libraries: ${XAPIAN_LIBRARIES}")
message(STATUS "    include path: ${XAPIAN_INCLUDE_DIR}")
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
include_directories(${XAPIAN_INCLUDE_DIR})
include_directories(${LIBEXIF_INCLUDEDIR})
# opencv module quality
include_directories(${CMAKE_BINARY_DIR}/_deps/opencv_contrib-src/modules/quality/include)

