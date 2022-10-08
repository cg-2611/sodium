# find llvm version 15 as it is required by the sodium compiler
find_package(LLVM 15 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}: ${LLVM_DIR}")

foreach(target ${LLVM_TARGETS_TO_BUILD})
    list(APPEND targets ${target})
endforeach()


# add llvm libraries to sodium compiler library target
target_include_directories(libnac PRIVATE ${LLVM_INCLUDE_DIRS})
llvm_map_components_to_libnames(LLVM_LIBS support core irreader ${targets})
target_link_libraries(libnac PRIVATE ${LLVM_LIBS})

if(BUILD_TESTS)
    add_subdirectory(googletest)

    # add googletest libraries to sodium compiler test target
    target_include_directories(nac_test PRIVATE googletest/googletest/include googletest/googlemock/include)
    target_link_directories(nac_test PRIVATE ${CMAKE_BINARY_DIR}/lib)
    target_link_libraries(nac_test PRIVATE gtest gtest_main gmock gmock_main)
endif()
