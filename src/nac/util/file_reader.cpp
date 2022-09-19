#include "sodium/nac/util/file_reader.h"

#include <fstream>
#include <memory>
#include <optional>
#include <string>

namespace sodium::util {

std::optional<std::string> read_file(const std::string &file_path) {
    auto file = std::ifstream(file_path, std::ios::ate);

    if (!file.is_open()) {
        return {};
    }

    auto file_size = file.tellg();
    file.seekg(std::ios::beg);

    auto file_contents = std::string(file_size, '\0');
    file.read(file_contents.data(), file_size);

    if (file.fail()) {
        return {};
    }

    return file_contents;
}

} // namespace sodium::util
