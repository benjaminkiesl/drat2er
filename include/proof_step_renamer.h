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

#ifndef DRAT2ER_PROOF_STEP_RENAMER_H_
#define DRAT2ER_PROOF_STEP_RENAMER_H_

#include <string>
#include <unordered_map>
#include "proof_transformer.h"

namespace drat2er{

class Deletion;
class Clause;
class RatClause;
class RupClause;

// A ProofStepRenamer takes an input LRAT proof whose instructions are not
// necessarily numbered in ascending order. It then outputs a new LRAT proof
// that is obtained by renaming the instructions of the original proof in
// ascending order.
class ProofStepRenamer : public ProofTransformer {

 public:
  ProofStepRenamer(const int index_of_first_proof_step = 1, 
                   const bool is_verbose=false);

 protected:
  virtual void HandleRupAddition(const RupClause& rup) override;
  virtual void HandleExtension(const Clause& definition_clause) override;

 private:
  int GetNextClauseIndex();
  int RenameClauseIndex(const int clause_index) const;
  void UpdateRenaming(const int old_clause_index, const int new_clause_index); 
  RupClause RenamePositiveHints(const RupClause& clause) const;

  std::unordered_map<int, int> clause_renaming_;
  int index_of_next_step_;
};

} // namespace drat2er

#endif
