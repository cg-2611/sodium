#ifndef SODIUM_NAC_IO_FILE_H
#define SODIUM_NAC_IO_FILE_H

#include <string>

namespace sodium {

/**
 * Used to store information about a file, such as its path, size and contents.
 */
class File {
public:
    /**
     * Constructor for File. The contents of the file are read and stored along with the size of the file
     * when this is called.
     * @param path The path to the file.
     */
    File(std::string path);

    /**
     * Destructor for File.
     */
    ~File() = default;

    /**
     * @return The path of the file as a string.
     */
    const std::string &path() const noexcept;

    /**
     * @return The contents of the file as a string.
     */
    const std::string &contents() const noexcept;

    /**
     * @return The size of the file in bytes.
     */
    size_t size() const noexcept;

private:
    std::string path_;
    std::string contents_;
    size_t size_;
};

} // namespace sodium

#endif // SODIUM_NAC_IO_FILE_H
