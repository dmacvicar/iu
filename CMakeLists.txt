cmake_minimum_required(VERSION 3.17)
project(iu)

set(IMAGE_METADATA_BACKEND "exiv2" CACHE STRING "Image Metada" )

add_subdirectory(ext)
add_subdirectory(src)

