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

#include "proof_step_renamer.h"
#include "clause.h"
#include "rat_clause.h"
#include "rup_clause.h"
#include "lrat_parser.h"
#include "deletion.h"

using std::string;
using std::endl;

namespace drat2er
{

ProofStepRenamer::ProofStepRenamer(int index_of_first_proof_step, 
                                   bool is_verbose) 
                           : ProofTransformer(
                               "Renaming proof steps incrementally.", 
                               is_verbose),
                             clause_renaming_{},
                             index_of_next_step_{index_of_first_proof_step}{}

void ProofStepRenamer::HandleExtension(const Clause& extension){
  Clause renamed_extension(extension);
  renamed_extension.SetIndex(GetNextClauseIndex());
  UpdateRenaming(extension.GetIndex(), renamed_extension.GetIndex());
  WriteClauseToOutput(renamed_extension);
}

void ProofStepRenamer::HandleRupAddition(const RupClause& rup){
  auto renamed_rup = RenamePositiveHints(rup);
  int old_index = rup.GetIndex();
  renamed_rup.SetIndex(GetNextClauseIndex());
  UpdateRenaming(old_index, renamed_rup.GetIndex());
  WriteClauseToOutput(renamed_rup);
}

int ProofStepRenamer::GetNextClauseIndex(){
  return index_of_next_step_++;
}

int ProofStepRenamer::RenameClauseIndex(const int clause_index) const {
  if(clause_renaming_.find(clause_index) != clause_renaming_.end()){
    return clause_renaming_.at(clause_index);
  }
  return clause_index;
}

void ProofStepRenamer::UpdateRenaming(const int old_clause_index, 
                                      const int new_clause_index){
  clause_renaming_[old_clause_index] = new_clause_index;
}

RupClause ProofStepRenamer::RenamePositiveHints(const RupClause& clause){
  RupClause renamed_clause(clause);
  renamed_clause.ClearPositiveHints();
  for(auto hint : clause.GetPositiveHints()){
    renamed_clause.AddPositiveHint(RenameClauseIndex(hint));
  }
  return renamed_clause;
}

} // namespace drat2er
