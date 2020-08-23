include(FetchContent)

# Command line parsing
FetchContent_Declare(cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(cli11)

# Formatting library (also used by spdlog)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(fmt)

# Logging library
FetchContent_Declare(spdlog
  URL https://github.com/gabime/spdlog/archive/v1.5.0.tar.gz
  URL_HASH SHA256=b38e0bbef7faac2b82fed550a0c19b0d4e7f6737d5321d4fd8f216b80f8aee8a
)
FetchContent_MakeAvailable(spdlog)
add_definitions(-DSPDLOG_FMT_EXTERNAL)

# CSV2 parser
FetchContent_Declare(csv2
  GIT_REPOSITORY https://github.com/p-ranav/csv2.git
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(csv2)

# Test library
FetchContent_Declare(catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(catch2)

# Quality (BRISQUE) opencv module and the model included in samples
FetchContent_Declare(opencv_contrib
  GIT_REPOSITORY https://github.com/opencv/opencv_contrib.git
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(opencv_contrib)

