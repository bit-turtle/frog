cmake_minimum_required(VERSION 3.28)
project(Frogicol LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Get SFML
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Frog Client
add_executable(frogclient src/frogclient.cpp)
target_link_libraries(frogclient PRIVATE sfml-network)
target_compile_features(frogclient PRIVATE cxx_std_17)

# Frog Server
add_executable(frogserver src/frogserver.cpp)
target_link_libraries(frogserver PRIVATE sfml-network)
target_compile_features(frogserver PRIVATE cxx_std_17)
