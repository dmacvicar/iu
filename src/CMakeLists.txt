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
  index_${IMAGE_METADATA_BACKEND}.cpp
  gps.cpp
  detect_objects.cpp
  )

add_library(libiu STATIC ${libiu_SOURCES})
set_target_properties(libiu PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(libiu xapian)
if(IMAGE_METADATA_BACKEND STREQUAL "exiv2")
  target_link_libraries(libiu ${LIBEXIV2_LIBRARIES})
elseif (IMAGE_METADATA_BACKEND STREQUAL "libexif")
  target_link_libraries(libiu ${LIBEXIF_LIBRARIES})
endif()
# FIXME not working
#target_link_libraries(libiu ${OpenCV_LIBS})
target_link_libraries(libiu opencv_core opencv_dnn opencv_imgcodecs)

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
include_directories(${nanoflann_SOURCE_DIR}/include)
include_directories(${csv2_SOURCE_DIR}/include)
include_directories(${tl-expected_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})
