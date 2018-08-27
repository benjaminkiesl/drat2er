#ifndef DRAT2ER_FILE_HELPER_H
#define DRAT2ER_FILE_HELPER_H

#include <string>

namespace drat2er
{

namespace file_helper
{

// Returns the current working directory.
std::string get_current_working_directory();

// Returns the number of lines in the file at the given path.
int get_number_of_lines(std::string file_path);

} // namespace file_helper

} // namespace drat2er

#endif
