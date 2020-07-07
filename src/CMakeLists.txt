if(TIDY)
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.;
    -checks=cppcoreguidelines-*, portability-*, readability-*;)
endif()

set(iu_SOURCES
  find.cpp
  index.cpp
  index_${IMAGE_METADATA_BACKEND}.cpp
  main.cpp
  )

add_executable(iu
  ${iu_SOURCES}
)
set_target_properties(iu PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(iu xapian)

if(IMAGE_METADATA_BACKEND STREQUAL "exiv2")
  target_link_libraries(iu ${LIBEXIV2_LIBRARIES})
elseif (IMAGE_METADATA_BACKEND STREQUAL "libexif")
  target_link_libraries(iu ${LIBEXIF_LIBRARIES})
endif()

include_directories(${CLI11_SOURCE_DIR}/include)
include_directories(${spdlog_SOURCE_DIR}/include)
include_directories(${nanoflann_SOURCE_DIR}/include)
