cmake_minimum_required(VERSION 3.28.3)
project(rust2cpp VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Corrosion)

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    # v0.6 will be updated to point to the latest patch version.
    # Use v0.6.<patch_version> or the commit hash to prevent such auto updates.
    GIT_TAG v0.6.1
)

set(Rust_TOOLCHAIN "1.95.0-x86_64-unknown-linux-gnu")
# Set any global configuration variables such as `Rust_TOOLCHAIN` before this line!
FetchContent_MakeAvailable(Corrosion)

corrosion_import_crate(MANIFEST_PATH rust/Cargo.toml)
target_sources(rust_lib INTERFACE
        FILE_SET HEADERS
        BASE_DIRS rust/target/include
        FILES
        rust/target/include/rust_lib.h
)

# Rust libraries must be installed using `corrosion_install`.
corrosion_install(TARGETS rust_lib EXPORT RustLibTarget)

add_executable(cpp-exe cpp/main.cpp)
target_link_libraries(cpp-exe PUBLIC rust_lib)

add_executable(cpp-exe-shared cpp/main.cpp)
target_link_libraries(cpp-exe-shared PUBLIC rust_lib-shared)

install(TARGETS cpp-exe cpp-exe-shared)

enable_testing()

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  INSTALL_COMMAND "" # Disable install for googletest
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(rust_lib_test cpp/test.cpp)
target_link_libraries(rust_lib_test PUBLIC rust_lib PRIVATE GTest::gtest_main)

add_executable(rust_lib_test-shared cpp/test.cpp)
target_link_libraries(rust_lib_test-shared PUBLIC rust_lib-shared PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(rust_lib_test)
gtest_discover_tests(rust_lib_test-shared)
