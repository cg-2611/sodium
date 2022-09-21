#ifndef SODIUM_NAC_NAC_H
#define SODIUM_NAC_NAC_H

namespace sodium {

class File;

/// --- UNFINISHED ---
/// Compiles a given sodium file.
/// \param file  The file to be compiled.
/// \return \c true if the file was compiled successfully, or \c false if there were problems diagnosed during the
/// compilation of the file.
bool compile_file(const File &file);

} // namespace sodium

#endif // SODIUM_NAC_NAC_H
