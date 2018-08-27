#ifndef DRAT2ER_DRAT_TRIM_H
#define DRAT2ER_DRAT_TRIM_H

#include <string>
#include <vector>

namespace drat2er
{

namespace drat_trim
{

int check_and_convert_to_lrat(std::string input_formula_path,
                              std::string input_proof_path,
                              std::string output_proof_path, bool is_verbose);

} // namespace drat_trim

} // namespace drat2er

#endif
