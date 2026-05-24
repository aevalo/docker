cmake_minimum_required(VERSION 3.28.3)
project(rust2cpp VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  message(WARNING "CMAKE_BUILD_TYPE not defined, defaulting to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

FetchContent_Declare(
  Corrosion
  URL           https://github.com/corrosion-rs/corrosion/archive/refs/tags/v0.6.1.zip
  URL_HASH      SHA256=5c1c5c6ab7196fe4466321c14e9698ec2d0a977c89fd9f1639a37fcffdf604f2
  DOWNLOAD_DIR  "${PROJECT_SOURCE_DIR}/vendor"
  DOWNLOAD_NAME "corrosion-v0.6.1.zip"
)

set(Rust_TOOLCHAIN "stable-x86_64-unknown-linux-gnu" CACHE STRING "Rust toolchain used by Corrosion")
# Set any global configuration variables such as `Rust_TOOLCHAIN` before this line!
FetchContent_MakeAvailable(Corrosion)

list(APPEND RELEASE_BUILD_TYPES "Release" "RelWithDebInfo" "MinSizeRel")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CARGO_PROFILE "dev" CACHE STRING "Cargo profile used by Corrosion")
elseif (CMAKE_BUILD_TYPE IN_LIST RELEASE_BUILD_TYPES)
  set(CARGO_PROFILE "release" CACHE STRING "Cargo profile used by Corrosion")
else()
  message(WARNING "Unrecognized CMAKE_BUILD_TYPE not defined, defaulting to dev profile for Corrosion")
  set(CARGO_PROFILE "dev" CACHE STRING "Cargo profile used by Corrosion")
endif()

corrosion_import_crate(
  MANIFEST_PATH rust/Cargo.toml
  PROFILE ${CARGO_PROFILE}
)

# Add a manually written header file which will be exported
target_sources(rust_lib INTERFACE
        FILE_SET HEADERS
        BASE_DIRS rust/include
        FILES
        rust/include/rust_lib/rust_lib.h
)

# Rust libraries must be installed using `corrosion_install`.
corrosion_install(TARGETS rust_lib EXPORT RustLibTarget)

# Installs the main target
install(
        EXPORT RustLibTarget
        NAMESPACE RustLib::
        DESTINATION lib/cmake/RustLib
)

add_executable(cpp-exe cpp/main.cpp)
add_dependencies(cpp-exe rust_lib)
target_include_directories(cpp-exe PUBLIC rust/include)
target_link_libraries(cpp-exe PUBLIC rust_lib)

add_executable(cpp-exe-shared cpp/main.cpp)
add_dependencies(cpp-exe-shared rust_lib-shared)
target_include_directories(cpp-exe-shared PUBLIC rust/include)
target_link_libraries(cpp-exe-shared PUBLIC rust_lib-shared)

install(TARGETS cpp-exe cpp-exe-shared)

enable_testing()

FetchContent_Declare(
  googletest
  URL           https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  URL_HASH      SHA256=40d4ec942217dcc84a9ebe2a68584ada7d4a33a8ee958755763278ea1c5e18ff
  DOWNLOAD_DIR  "${PROJECT_SOURCE_DIR}/vendor"
  DOWNLOAD_NAME "googletest-v1.17.0.zip"
  EXCLUDE_FROM_ALL
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(rust_lib_test cpp/test.cpp)
target_include_directories(rust_lib_test PUBLIC rust/include)
target_link_libraries(rust_lib_test PUBLIC rust_lib PRIVATE GTest::gtest_main)

add_executable(rust_lib_test-shared cpp/test.cpp)
target_include_directories(rust_lib_test-shared PUBLIC rust/include)
target_link_libraries(rust_lib_test-shared PUBLIC rust_lib-shared PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(rust_lib_test)
gtest_discover_tests(rust_lib_test-shared)
