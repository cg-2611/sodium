add_subdirectory(googletest)

# add googletest libraries to sodium compiler test target
target_include_directories(nac_test PRIVATE googletest/googletest/include googletest/googlemock/include)
target_link_directories(nac_test PRIVATE ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(nac_test PRIVATE gtest gtest_main gmock gmock_main)
