#ifndef SODIUM_NAC_IO_FILE_H
#define SODIUM_NAC_IO_FILE_H

#include <string>

namespace sodium::nac {

/**
 * This class is used to stored information about a file, such as its path, size and contents.
*/
class File {
public:
    /**
     * Constructor for File. The contents of the file are read and stored along with the size of the file
     * when this is called.
     * @param filePath the path to the file.
     * @throws NACException when opening or reading the file fails.
    */
    File(const std::string &filePath);

    /**
     * Destructor for File.
    */
    ~File() {}

    /**
     * @return The path of the file as a string.
    */
    const std::string getPath() const;

    /**
     * @return The contents of the as a string.
    */
    const std::string getContents() const;

    /**
     * @return The size of the file in bytes.
    */
    size_t getSize() const;

private:
    std::string path_;
    std::string contents_;
    size_t size_;
};

} // namespace sodium::nac

#endif // SODIUM_NAC_IO_FILE_H
