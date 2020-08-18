
message(STATUS "Downloading bvlc_googlenet.caffemodel to ${CMAKE_BINARY_DIR}")
file(DOWNLOAD http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel
  ${CMAKE_BINARY_DIR}/bvlc_googlenet.caffemodel
  EXPECTED_HASH SHA256=6f7101e3a2183738a7125a0c5021ba82a1feb4228c5ca0924d991b6daf6f6fad
  SHOW_PROGRESS
)

message(STATUS "Downloading bvlc_googlenet.prototxt")
file(DOWNLOAD https://raw.githubusercontent.com/opencv/opencv_extra/master/testdata/dnn/bvlc_googlenet.prototxt
  ${CMAKE_BINARY_DIR}/bvlc_googlenet.prototxt
  EXPECTED_HASH SHA256=3cf5576e43eaf70fdd8b599d0dabc4825c318df49ff9430d6c48c3a3668d9476
  SHOW_PROGRESS
)

message(STATUS "Downloading image-net words.txt")
file(DOWNLOAD http://www.image-net.org/archive/words.txt
  ${CMAKE_BINARY_DIR}/words.txt
  EXPECTED_HASH SHA256=55df3c6ee75859de344e2834ef65d88d61cec389d57e787be59e942c9b73e9aa
  SHOW_PROGRESS
)

message(STATUS "Downloading image-net wordnet.is_a.txt")
file(DOWNLOAD http://www.image-net.org/archive/wordnet.is_a.txt
  ${CMAKE_BINARY_DIR}/wordnet.is_a.txt
  EXPECTED_HASH SHA256=ff7206572ae989d1de8acbd6cbd921854fefd32dcdcb50eab11b81e797e28cd1
  SHOW_PROGRESS
)

message(STATUS "Downloading reverse_geocode geocode.csv")
file(DOWNLOAD https://github.com/richardpenman/reverse_geocode/raw/f98a92cb072ab1509ae3a0289f5d4b27f7c268cd/geocode.csv
  ${CMAKE_BINARY_DIR}/geocode.txt
  EXPECTED_HASH SHA256=47ad6608f0fa4f4ad9f31a77444cb12d2875739c1b094f2f1521e953af471f46
  SHOW_PROGRESS
)

message(STATUS "Downloading reverse_geocode countries.csv")
file(DOWNLOAD https://github.com/richardpenman/reverse_geocode/raw/f98a92cb072ab1509ae3a0289f5d4b27f7c268cd/countries.csv
  ${CMAKE_BINARY_DIR}/countries.csv
  EXPECTED_HASH SHA256=aab6511e5d68e4ac7324fe9393de67de4bc94af154e97949f331f8c47cbb61fd
  SHOW_PROGRESS
)
