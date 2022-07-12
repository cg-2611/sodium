target_compile_features(nac PRIVATE cxx_std_17)
target_compile_options(nac PRIVATE -Wall -Wextra -Wpedantic)

target_sources(nac
    PRIVATE
        nac/main.cpp
        nac/nac.cpp
        nac/error/error.cpp
        nac/io/file.cpp
        nac/io/file_io.cpp
)

target_include_directories(nac PRIVATE ${CMAKE_SOURCE_DIR}/include)
