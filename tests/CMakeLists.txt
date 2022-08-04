# for nac_test target
add_executable(nac_test)
target_compile_features(nac_test PRIVATE cxx_std_20)
target_include_directories(nac_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(nac_test PRIVATE libnac)
target_sources(nac_test
    PRIVATE
        nac/io/file_io_test.cpp
        nac/lexer/lexer_test.cpp
        nac/util/string_formatter_test.cpp
)

add_test(NAME nac_test COMMAND nac_test)
