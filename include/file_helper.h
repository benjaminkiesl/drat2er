#ifndef DRAT2ER_FILE_HELPER_H
#define DRAT2ER_FILE_HELPER_H

#include <string>

namespace drat2er{

namespace file_helper {

  std::string get_current_working_directory();
  int get_number_of_lines(std::string file_path);

} // namespace file_helper

} // namespace drat2er

#endif
