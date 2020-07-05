include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEXIF REQUIRED libexif)

add_executable(indexer indexer.cpp)
set_target_properties(indexer PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(indexer xapian)
target_link_libraries(indexer ${LIBEXIF_LIBRARIES})

add_executable(search search.cpp)
set_target_properties(search PROPERTIES
  CXX_STANDARD 17
)
target_link_libraries(search xapian)
