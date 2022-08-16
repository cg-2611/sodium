# for nac_test target
add_executable(nac_test)
target_compile_features(nac_test PRIVATE cxx_std_20)
target_include_directories(nac_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(nac_test PRIVATE libnac)

add_subdirectory(nac/io)
add_subdirectory(nac/lexer)
add_subdirectory(nac/parser)
add_subdirectory(nac/util)

add_test(NAME nac_test COMMAND nac_test)
