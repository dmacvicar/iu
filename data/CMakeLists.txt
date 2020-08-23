include(FetchContent)
# Reverse geolocation data files (.csv)
FetchContent_Declare(reverse_geocode
  GIT_REPOSITORY https://github.com/richardpenman/reverse_geocode
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(reverse_geocode)
FetchContent_GetProperties(reverse_geocode
  SOURCE_DIR REVERSE_GEOCODE_SOURCE_DIR
)

# Module is fetched as part of ext/, so we just grab the data files
FetchContent_GetProperties(opencv_contrib
  SOURCE_DIR OPENCV_CONTRIB_SOURCE_DIR
)

FetchContent_Declare(ilsvrc12
  URL http://dl.caffe.berkeleyvision.org/caffe_ilsvrc12.tar.gz
  URL_HASH SHA256=e35c0c1994a21f7d8ed49d01881ce17ab766743d3b0372cdc0183ff4d0dfc491
)
FetchContent_MakeAvailable(ilsvrc12)
FetchContent_GetProperties(ilsvrc12
  SOURCE_DIR ILSVRC12_SOURCE_DIR
)

add_custom_command(
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/bvlc_googlenet.prototxt
  ${CMAKE_CURRENT_BINARY_DIR}/bvlc_googlenet.caffemodel
  ${CMAKE_CURRENT_BINARY_DIR}/brisque_model_live.yml
  ${CMAKE_CURRENT_BINARY_DIR}/brisque_range_live.yml
  ${CMAKE_CURRENT_BINARY_DIR}/coutries.csv
  ${CMAKE_CURRENT_BINARY_DIR}/geocode.csv
  ${CMAKE_CURRENT_BINARY_DIR}/words.txt
  ${CMAKE_CURRENT_BINARY_DIR}/wordnet.is_a.txt
  ${CMAKE_CURRENT_BINARY_DIR}/synsets.txt
  COMMAND ${CMAKE_COMMAND}
  -DREVERSE_GEOCODE_SOURCE_DIR=${REVERSE_GEOCODE_SOURCE_DIR}
  -DOPENCV_CONTRIB_SOURCE_DIR=${OPENCV_CONTRIB_SOURCE_DIR}
  -DILSVRC12_SOURCE_DIR=${ILSVRC12_SOURCE_DIR}
  -P ${CMAKE_CURRENT_SOURCE_DIR}/download_data.cmake
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(Python3)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/words_full_hierarchy.txt
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate.py --output ${CMAKE_CURRENT_BINARY_DIR}/words_full_hierarchy.txt
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/words.txt ${CMAKE_CURRENT_BINARY_DIR}/wordnet.is_a.txt ${CMAKE_CURRENT_BINARY_DIR}/synsets.txt
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(data
  DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/words_full_hierarchy.txt
  ${CMAKE_CURRENT_BINARY_DIR}/bvlc_googlenet.prototxt
  ${CMAKE_CURRENT_BINARY_DIR}/bvlc_googlenet.caffemodel
  ${CMAKE_CURRENT_BINARY_DIR}/geocode.csv
  ${CMAKE_CURRENT_BINARY_DIR}/countries.csv
  ${CMAKE_CURRENT_BINARY_DIR}/wordnet.is_a.txt
  ${CMAKE_CURRENT_BINARY_DIR}/words.txt
  ${CMAKE_CURRENT_BINARY_DIR}/synsets.txt
  ${CMAKE_CURRENT_BINARY_DIR}/brisque_model_live.yml
  ${CMAKE_CURRENT_BINARY_DIR}/brisque_range_live.yml
)

