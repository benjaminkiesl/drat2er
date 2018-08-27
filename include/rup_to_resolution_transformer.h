#ifndef DRAT2ER_RUP_TO_RESOLUTION_TRANSFORMER_H_
#define DRAT2ER_RUP_TO_RESOLUTION_TRANSFORMER_H_

#include <memory>
#include <iostream>
#include "proof_transformer.h"

namespace drat2er
{

class Formula;
class RatClause;
class RupClause;
class Deletion;

// Takes an LRAT proof that only contains RUP additions and extensions
// (definition introductions) and transforms all RUP additions into resolution
// chains. The output can be printed either in the TRACECHECK format or
// the DRAT format.
class RupToResolutionTransformer : public ProofTransformer
{

 public:
  // If the parameter 'is_output_drat' is true, the resulting proof will
  // be encoded in the DRAT format, otherwise it will be encoded in the
  // TRACECHECK format. If print_progress is true, a status bar will be
  // printed on the standard output to indicate the progress.
  RupToResolutionTransformer(std::shared_ptr<Formula> formula,
                             bool is_output_drat = false,
                             bool print_progress = false);

 protected:
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleExtension(const Clause& extension) override;

 private:
  std::shared_ptr<Formula> formula_;
  bool found_empty_clause_;
};

} // namespace

#endif
