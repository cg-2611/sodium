#ifndef SODIUM_NAC_IO_FILE_H
#define SODIUM_NAC_IO_FILE_H

#include <string>

namespace sodium {

/**
 * @brief Used to store information about a file, such as its path, size and contents.
 *
 */
class File {
public:
    /**
     * @brief Construct a new File object. The contents of the file are read and stored along with the size of the file
     * when this is called.
     *
     * @param path The path to the file as an std::string.
     */
    File(std::string path);

    /**
     * @brief Destroy the File object.
     *
     */
    ~File() = default;

    /**
     * @brief Get the path of this file.
     *
     * @return const std::string& that is the path of this file.
     */
    const std::string &path() const noexcept;

    /**
     * @brief Get the contents of this file.
     *
     * @return const std::string& that is the contents of this file.
     */
    const std::string &contents() const noexcept;

    /**
     * @brief Get the size of this file.
     *
     * @return size_t that is the size of this file in bytes.
     */
    size_t size() const noexcept;

private:
    std::string path_;
    std::string contents_;
    size_t size_;
};

} // namespace sodium

#endif // SODIUM_NAC_IO_FILE_H
