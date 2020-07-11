if(TIDY)
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    -checks=cppcoreguidelines-*, portability-*, readability-*;)
endif()

set(libiu_SOURCES
  find.cpp
  index.cpp
  index_${IMAGE_METADATA_BACKEND}.cpp
  gps.cpp
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

add_executable(iu
  main.cpp
)
set_target_properties(iu PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(iu libiu)

include_directories(${CLI11_SOURCE_DIR}/include)
include_directories(${spdlog_SOURCE_DIR}/include)
include_directories(${nanoflann_SOURCE_DIR}/include)
include_directories(${csv2_SOURCE_DIR}/include)
