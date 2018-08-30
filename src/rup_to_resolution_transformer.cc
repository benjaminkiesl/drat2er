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

#include "rup_to_resolution_transformer.h"
#include <string>
#include <memory>
#include <cassert>
#include "formula.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "deletion.h"
#include "instruction_serialization.h"

using std::string;
using std::endl;
using std::shared_ptr;

namespace drat2er
{

RupToResolutionTransformer::RupToResolutionTransformer(
    shared_ptr<Formula> formula,
    const bool is_output_drat,
    const bool print_progress)
    : ProofTransformer("Transforming RUPs to resolution chains.",
                       print_progress),
      formula_ {formula},
      is_output_drat_{is_output_drat},
      found_empty_clause_ {false} {}

void RupToResolutionTransformer::HandleExtension(const Clause& extension)
{
  if(!found_empty_clause_) {
    formula_->AddClause(extension);
    if(is_output_drat_) {
      OutputStream() << ToDRAT(extension) << endl;
    } else {
      OutputStream() << ToTRACECHECKExtension(extension) << endl;
    }
  }
}

void RupToResolutionTransformer::HandleRupAddition(const RupClause& rup)
{
  if(!found_empty_clause_) {
    Formula sub_formula;
    for(auto clause_index : rup.GetPositiveHints()) {
      assert(formula_->GetClause(clause_index) != nullptr);
      sub_formula.AddClause(*formula_->GetClause(clause_index));
    }
    auto subsuming_rup = sub_formula.DeriveSubsumingClause(rup);
    assert(subsuming_rup != nullptr);
    formula_->AddClause(*subsuming_rup);
    if(is_output_drat_){
      OutputStream() << ToDRAT(*subsuming_rup) << endl;
    } else {
      OutputStream() << ToTRACECHECK(*subsuming_rup) << endl;
    }
    if(subsuming_rup->empty()) {
      found_empty_clause_ = true;
    }
  }
}

} // namespace drat2er
