include(FetchContent)

FetchContent_Declare(cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(spdlog
  URL https://github.com/gabime/spdlog/archive/v1.5.0.tar.gz
  URL_HASH SHA256=b38e0bbef7faac2b82fed550a0c19b0d4e7f6737d5321d4fd8f216b80f8aee8a
)
FetchContent_MakeAvailable(spdlog)
add_definitions(-DSPDLOG_FMT_EXTERNAL)

FetchContent_Declare(csv2
  GIT_REPOSITORY https://github.com/p-ranav/csv2.git
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(csv2)

FetchContent_Declare(catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(catch2)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEXIF REQUIRED libexif)

find_package(OpenCV REQUIRED core dnn imgcodecs)
find_package(xapian REQUIRED)
