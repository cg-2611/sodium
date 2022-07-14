#ifndef SODIUM_NAC_IO_FILE_READER_H
#define SODIUM_NAC_IO_FILE_READER_H

#include <fstream>
#include <string>

namespace sodium::nac {

/**
 * This class can be used to read a file.
*/
class FileReader {
public:
    /**
     * Constructor for FileReader. A file stream is opened in binary mode and at the end of the stream
     * for the provided file when this is called.
     * @param filePath the path to the file to be read.
     * @throws NACException if opening the file fails.
    */
    FileReader(const std::string &filePath);

    /**
     * Destructor for FileReader. The file stream is closed when this is called.
    */
    ~FileReader();

    /**
     * Reads the contents of the file as a string using the file stream opened in the constructor.
     * @param fileContents [out] a string of the contents of the file.
     * @return The size of the file in bytes.
     * @throws NACException if the failbit or badbit of the file stream are set during the read.
    */
    size_t readFileContents(std::string &fileContents);

    /**
     * @return True if the file stream is open.
    */
    bool isFileStreamOpen() const;

private:
    std::string path_;
    std::ifstream fileStream_;


    void openFile();  // creates and opens fileStream_
    void closeFile(); // closes fileStream_
};

} // namespace sodium::nac

#endif // SODIUM_NAC_IO_FILE_READER_H
