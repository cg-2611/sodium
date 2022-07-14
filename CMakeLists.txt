cmake_minimum_required(VERSION 3.23)

project(sodium VERSION 0.0.0)

add_executable(nac)

add_subdirectory(src) # sources are added for sodium compiler executable in src

if(ENABLE_TESTS)
    # integrate googletest with ctest
    # including CTest module automatically invokes enable_testing()
    include(CTest)
    add_subdirectory(tests) # executable for sodium compiler tests is created in tests
endif()

add_subdirectory(third_party) # external libraries are linked to executbales in third_party
