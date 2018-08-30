// MIT License
//
// Copyright (c) 2018 Benjamin Kiesl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
// IN THE SOFTWARE.

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
                             const bool is_output_drat = false,
                             const bool print_progress = false);

 protected:
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleExtension(const Clause& extension) override;

 private:
  std::shared_ptr<Formula> formula_;
  bool is_output_drat_;
  bool found_empty_clause_;
};

} // namespace drat2er

#endif
