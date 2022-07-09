add_executable(nac_test)
target_compile_features(nac_test PRIVATE cxx_std_17)
target_sources(nac_test
    PRIVATE
        nac/main_test.cpp
)
target_include_directories(nac_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(
    NAME nac_test
    COMMAND nac_test
)
