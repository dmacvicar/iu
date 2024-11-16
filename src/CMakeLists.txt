include(FetchContent)
FetchContent_GetProperties(opencv_contrib
  SOURCE_DIR OPENCV_CONTRIB_SOURCE_DIR
)
if(TIDY)
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=${CMAKE_SOURCE_DIR}/src;
    -checks=cppcoreguidelines-*,portability-*,readability-*;)
endif()

set(libiu_SOURCES
  find.cpp
  index.cpp
  jpeg.cpp
  resources.cpp
  index_libexif.cpp
  location.cpp
  entities.cpp
  detect_quality.cpp
  ai.cpp
  base64.cpp
  # opencv contrib
  ${OPENCV_CONTRIB_SOURCE_DIR}/modules/quality/src/qualitybrisque.cpp
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
pkg_check_modules(LIBCURL REQUIRED IMPORTED_TARGET libcurl)
message(STATUS "CURL library status:")
message(STATUS "    libraries: ${LIBCURL_LIBRARIES}")
message(STATUS "    include path: ${LIBCURL_INCLUDE_DIRS}")
target_link_libraries(libiu ${LIBCURL_LIBRARIES})

add_executable(iu
  main.cpp
)
set_target_properties(iu PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(iu PRIVATE libiu)
add_dependencies(iu data)

include_directories(${CLI11_SOURCE_DIR}/include)
include_directories(${FMT_SOURCE_DIR}/include)
include_directories(${spdlog_SOURCE_DIR}/include)
include_directories(${csv2_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${XAPIAN_INCLUDE_DIR})
include_directories(${LIBEXIF_INCLUDEDIR})
# opencv module quality
include_directories(${CMAKE_BINARY_DIR}/_deps/opencv_contrib-src/modules/quality/include)
include_directories(${OPENCV_CONTRIB_SOURCE_DIR}/modules/quality/include)
include_directories(${openaicpp_SOURCE_DIR}/include/openai)

