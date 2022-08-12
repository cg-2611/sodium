# for libnac target
add_library(libnac)
target_compile_features(libnac PRIVATE cxx_std_20)
target_compile_options(libnac PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(libnac PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(libnac
    PRIVATE
        nac/nac.cpp
)

add_subdirectory(nac/errors)
add_subdirectory(nac/exceptions)
add_subdirectory(nac/io)
add_subdirectory(nac/lexer)
add_subdirectory(nac/parser)

# for nac target
target_compile_features(nac PRIVATE cxx_std_20)
target_compile_options(nac PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(nac PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(nac PRIVATE libnac)
target_sources(nac PRIVATE nac/main.cpp)
