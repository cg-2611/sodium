# for naclibtarget
add_library(naclib)
target_compile_features(naclib PRIVATE cxx_std_17)
target_compile_options(naclib PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(naclib PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_sources(naclib
    PRIVATE
        nac/nac.cpp
        nac/error/error.cpp
        nac/io/file.cpp
        nac/io/file_reader.cpp
)

# for nac target
target_compile_features(nac PRIVATE cxx_std_17)
target_compile_options(nac PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(nac PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(nac PRIVATE naclib)
target_sources(nac PRIVATE nac/main.cpp)
