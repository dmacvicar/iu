message(STATUS "Copying geocode data to ${CMAKE_CURRENT_BINARY_DIR}")
file(COPY
  ${REVERSE_GEOCODE_SOURCE_DIR}/geocode.csv
  ${REVERSE_GEOCODE_SOURCE_DIR}/countries.csv
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

message(STATUS "Copying ILSVRC12 synset list to ${CMAKE_CURRENT_BINARY_DIR}")
file(COPY
  ${ILSVRC12_SOURCE_DIR}/synsets.txt
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

message(STATUS "Copying brisque models to ${CMAKE_CURRENT_BINARY_DIR}")
file(COPY
  ${OPENCV_CONTRIB_SOURCE_DIR}/modules/quality/samples/brisque_model_live.yml
  ${OPENCV_CONTRIB_SOURCE_DIR}/modules/quality/samples/brisque_range_live.yml
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

message(STATUS "Downloading bvlc_googlenet to ${CMAKE_CURRENT_BINARY_DIR}")
file(DOWNLOAD http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel
  ${CMAKE_CURRENT_BINARY_DIR}/bvlc_googlenet.caffemodel
  EXPECTED_HASH SHA256=6f7101e3a2183738a7125a0c5021ba82a1feb4228c5ca0924d991b6daf6f6fad
  SHOW_PROGRESS
)
file(DOWNLOAD https://raw.githubusercontent.com/opencv/opencv_extra/master/testdata/dnn/bvlc_googlenet.prototxt
  ${CMAKE_CURRENT_BINARY_DIR}/bvlc_googlenet.prototxt
  EXPECTED_HASH SHA256=3cf5576e43eaf70fdd8b599d0dabc4825c318df49ff9430d6c48c3a3668d9476
  SHOW_PROGRESS
)

message(STATUS "Downloading image-net words.txt & wordnet.is_a.txt")
file(DOWNLOAD http://www.image-net.org/archive/words.txt
  ${CMAKE_CURRENT_BINARY_DIR}/words.txt
  EXPECTED_HASH SHA256=55df3c6ee75859de344e2834ef65d88d61cec389d57e787be59e942c9b73e9aa
  SHOW_PROGRESS
)
file(DOWNLOAD http://www.image-net.org/archive/wordnet.is_a.txt
  ${CMAKE_CURRENT_BINARY_DIR}/wordnet.is_a.txt
  EXPECTED_HASH SHA256=ff7206572ae989d1de8acbd6cbd921854fefd32dcdcb50eab11b81e797e28cd1
  SHOW_PROGRESS
)
