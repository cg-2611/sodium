cmake_minimum_required(VERSION 3.23)

project(sodium VERSION 0.0.0)

add_executable(nac)

# for libnac target
add_library(libnac)
target_compile_features(libnac PRIVATE cxx_std_20)
target_compile_options(libnac PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(libnac PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src) # sources are added for sodium compiler executable in src

if(BUILD_TESTS)
    # integrate googletest with ctest
    # including CTest module automatically invokes enable_testing()
    include(CTest)
    add_subdirectory(tests) # executable for sodium compiler tests is created in tests
endif()

add_subdirectory(third_party) # external libraries are linked to executbales in third_party
