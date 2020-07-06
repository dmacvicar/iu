add_executable(iu
  find.cpp
  index.cpp
  main.cpp
)
set_target_properties(iu PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(iu xapian)
target_link_libraries(iu ${LIBEXIF_LIBRARIES})

include_directories(${CLI11_SOURCE_DIR}/include)
include_directories(${spdlog_SOURCE_DIR}/include)
include_directories(${nanoflann_SOURCE_DIR}/include)
